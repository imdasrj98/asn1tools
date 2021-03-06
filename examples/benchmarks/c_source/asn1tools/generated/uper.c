/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2018-2019 Erik Moqvist
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * This file was generated by asn1tools version 0.145.2 Thu Jan 24 08:14:06 2019.
 */

#include <string.h>

#include "uper.h"

struct encoder_t {
    uint8_t *buf_p;
    ssize_t size;
    ssize_t pos;
};

struct decoder_t {
    const uint8_t *buf_p;
    ssize_t size;
    ssize_t pos;
};

static void encoder_init(struct encoder_t *self_p,
                         uint8_t *buf_p,
                         size_t size)
{
    self_p->buf_p = buf_p;
    self_p->size = (8 * (ssize_t)size);
    self_p->pos = 0;
}

static ssize_t encoder_get_result(struct encoder_t *self_p)
{
    if (self_p->size >= 0) {
        return ((self_p->pos + 7) / 8);
    } else {
        return (self_p->pos);
    }
}

static void encoder_abort(struct encoder_t *self_p,
                          ssize_t error)
{
    if (self_p->size >= 0) {
        self_p->size = -error;
        self_p->pos = -error;
    }
}

static ssize_t encoder_alloc(struct encoder_t *self_p,
                             size_t size)
{
    ssize_t pos;

    if (self_p->pos + (ssize_t)size <= self_p->size) {
        pos = self_p->pos;
        self_p->pos += (ssize_t)size;
    } else {
        pos = -ENOMEM;
        encoder_abort(self_p, ENOMEM);
    }

    return (pos);
}

static void encoder_append_bit(struct encoder_t *self_p,
                               int value)
{
    ssize_t pos;

    pos = encoder_alloc(self_p, 1);

    if (pos < 0) {
        return;
    }

    if ((pos % 8) == 0) {
        self_p->buf_p[pos / 8] = 0;
    }

    self_p->buf_p[pos / 8] |= (value << (7 - (pos % 8)));
}

static void encoder_append_bytes(struct encoder_t *self_p,
                                 const uint8_t *buf_p,
                                 size_t size)
{
    size_t i;
    ssize_t pos;
    size_t byte_pos;
    size_t pos_in_byte;

    pos = encoder_alloc(self_p, 8 * size);

    if (pos < 0) {
        return;
    }

    byte_pos = ((size_t)pos / 8);
    pos_in_byte = ((size_t)pos % 8);

    if (pos_in_byte == 0) {
        memcpy(&self_p->buf_p[byte_pos], buf_p, size);
    } else {
        for (i = 0; i < size; i++) {
            self_p->buf_p[byte_pos + i] |= (buf_p[i] >> pos_in_byte);
            self_p->buf_p[byte_pos + i + 1] = (buf_p[i] << (8 - pos_in_byte));
        }
    }
}

static void encoder_append_uint32(struct encoder_t *self_p,
                                  uint32_t value)
{
    uint8_t buf[4];

    buf[0] = (value >> 24);
    buf[1] = (value >> 16);
    buf[2] = (value >> 8);
    buf[3] = value;

    encoder_append_bytes(self_p, &buf[0], sizeof(buf));
}

static void encoder_append_int32(struct encoder_t *self_p,
                                 int32_t value)
{
    value += 2147483648;
    encoder_append_uint32(self_p, (uint32_t)value);
}

static void encoder_append_bool(struct encoder_t *self_p, bool value)
{
    encoder_append_bit(self_p, value ? 1 : 0);
}

static void encoder_append_non_negative_binary_integer(struct encoder_t *self_p,
                                                       uint64_t value,
                                                       size_t size)
{
    size_t i;

    for (i = 0; i < size; i++) {
        encoder_append_bit(self_p, (value >> (size - i - 1)) & 1);
    }
}

static void decoder_init(struct decoder_t *self_p,
                         const uint8_t *buf_p,
                         size_t size)
{
    self_p->buf_p = buf_p;
    self_p->size = (8 * (ssize_t)size);
    self_p->pos = 0;
}

static ssize_t decoder_get_result(struct decoder_t *self_p)
{
    if (self_p->size >= 0) {
        return ((self_p->pos + 7) / 8);
    } else {
        return (self_p->pos);
    }
}

static void decoder_abort(struct decoder_t *self_p,
                          ssize_t error)
{
    if (self_p->size >= 0) {
        self_p->size = -error;
        self_p->pos = -error;
    }
}

static ssize_t decoder_free(struct decoder_t *self_p,
                            size_t size)
{
    ssize_t pos;

    if (self_p->pos + (ssize_t)size <= self_p->size) {
        pos = self_p->pos;
        self_p->pos += (ssize_t)size;
    } else {
        pos = -EOUTOFDATA;
        decoder_abort(self_p, EOUTOFDATA);
    }

    return (pos);
}

static int decoder_read_bit(struct decoder_t *self_p)
{
    ssize_t pos;
    int value;

    pos = decoder_free(self_p, 1);

    if (pos >= 0) {
        value = ((self_p->buf_p[pos / 8] >> (7 - (pos % 8))) & 1);
    } else {
        value = 0;
    }

    return (value);
}

static void decoder_read_bytes(struct decoder_t *self_p,
                               uint8_t *buf_p,
                               size_t size)
{
    size_t i;
    ssize_t pos;
    size_t byte_pos;
    size_t pos_in_byte;

    pos = decoder_free(self_p, 8 * size);

    if (pos < 0) {
        return;
    }

    byte_pos = ((size_t)pos / 8);
    pos_in_byte = ((size_t)pos % 8);

    if (pos_in_byte == 0) {
        memcpy(buf_p, &self_p->buf_p[byte_pos], size);
    } else {
        for (i = 0; i < size; i++) {
            buf_p[i] = (self_p->buf_p[byte_pos + i] << pos_in_byte);
            buf_p[i] |= (self_p->buf_p[byte_pos + i + 1] >> (8 - pos_in_byte));
        }
    }
}

static uint32_t decoder_read_uint32(struct decoder_t *self_p)
{
    uint8_t buf[4];

    decoder_read_bytes(self_p, &buf[0], sizeof(buf));

    return (((uint32_t)buf[0] << 24)
            | ((uint32_t)buf[1] << 16)
            | ((uint32_t)buf[2] << 8)
            | (uint32_t)buf[3]);
}

static int32_t decoder_read_int32(struct decoder_t *self_p)
{
    int32_t value;

    value = (int32_t)decoder_read_uint32(self_p);
    value -= 2147483648;

    return (value);
}

static bool decoder_read_bool(struct decoder_t *self_p)
{
    return (decoder_read_bit(self_p) != 0);
}

static uint64_t decoder_read_non_negative_binary_integer(struct decoder_t *self_p,
                                                         size_t size)
{
    size_t i;
    uint64_t value;

    value = 0;

    for (i = 0; i < size; i++) {
        value <<= 1;
        value |= (uint64_t)decoder_read_bit(self_p);
    }

    return (value);
}

static void uper_my_protocol_d_encode_inner(
    struct encoder_t *encoder_p,
    const struct uper_my_protocol_d_t *src_p)
{
    encoder_append_non_negative_binary_integer(
        encoder_p,
        src_p->length - 1u,
        4);
    encoder_append_bytes(encoder_p,
                         &src_p->buf[0],
                         src_p->length);
}

static void uper_my_protocol_d_decode_inner(
    struct decoder_t *decoder_p,
    struct uper_my_protocol_d_t *dst_p)
{
    dst_p->length = decoder_read_non_negative_binary_integer(
        decoder_p,
        4);
    dst_p->length += 1;
    decoder_read_bytes(decoder_p,
                       &dst_p->buf[0],
                       dst_p->length);
}

static void uper_my_protocol_c_encode_inner(
    struct encoder_t *encoder_p,
    const struct uper_my_protocol_c_t *src_p)
{
    uint8_t i;
    uint8_t i_2;

    encoder_append_bool(encoder_p, src_p->is_a_present);

    if (src_p->is_a_present) {
        encoder_append_bool(encoder_p, src_p->a.is_b_present);
        encoder_append_bool(encoder_p, src_p->a.c != 0);
        encoder_append_non_negative_binary_integer(
            encoder_p,
            src_p->a.a.length - 0u,
            3);

        for (i = 0; i < src_p->a.a.length; i++) {
            encoder_append_int32(encoder_p, src_p->a.a.elements[i]);
        }

        if (src_p->a.is_b_present) {
            encoder_append_bool(encoder_p, src_p->a.b);
        }

        if (src_p->a.c != 0) {
            encoder_append_non_negative_binary_integer(
                encoder_p,
                (uint64_t)(src_p->a.c - -40),
                7);
        }
    }

    encoder_append_uint32(encoder_p, src_p->b);

    switch (src_p->c.choice) {

    case uper_my_protocol_c_c_choice_a_e:
        encoder_append_non_negative_binary_integer(encoder_p, 0, 1);

        for (i_2 = 0; i_2 < 3; i_2++) {
            uper_my_protocol_d_encode_inner(encoder_p, &src_p->c.value.a.elements[i_2]);
        }

        break;

    case uper_my_protocol_c_c_choice_b_e:
        encoder_append_non_negative_binary_integer(encoder_p, 1, 1);
        encoder_append_bool(encoder_p, src_p->c.value.b);
        break;

    default:
        encoder_abort(encoder_p, EBADCHOICE);
        break;
    }

    encoder_append_bool(encoder_p, src_p->d.a);
}

static void uper_my_protocol_c_decode_inner(
    struct decoder_t *decoder_p,
    struct uper_my_protocol_c_t *dst_p)
{
    bool is_present;
    uint8_t i;
    uint8_t choice;
    uint8_t i_2;

    dst_p->is_a_present = decoder_read_bool(decoder_p);

    if (dst_p->is_a_present) {
        dst_p->a.is_b_present = decoder_read_bool(decoder_p);
        is_present = decoder_read_bool(decoder_p);
        dst_p->a.a.length = decoder_read_non_negative_binary_integer(
            decoder_p,
            3);
        dst_p->a.a.length += 0;

        if (dst_p->a.a.length > 5) {
            decoder_abort(decoder_p, EBADLENGTH);

            return;
        }

        for (i = 0; i < dst_p->a.a.length; i++) {
            dst_p->a.a.elements[i] = decoder_read_int32(decoder_p);
        }

        if (dst_p->a.is_b_present) {
            dst_p->a.b = decoder_read_bool(decoder_p);
        }

        if (is_present) {
            dst_p->a.c = decoder_read_non_negative_binary_integer(
                decoder_p,
                7);
            dst_p->a.c += -40;
        } else {
            dst_p->a.c = 0;
        }
    }

    dst_p->b = decoder_read_uint32(decoder_p);
    choice = decoder_read_non_negative_binary_integer(decoder_p, 1);

    switch (choice) {

    case 0:
        dst_p->c.choice = uper_my_protocol_c_c_choice_a_e;

        for (i_2 = 0; i_2 < 3; i_2++) {
            uper_my_protocol_d_decode_inner(decoder_p, &dst_p->c.value.a.elements[i_2]);
        }

        break;

    case 1:
        dst_p->c.choice = uper_my_protocol_c_c_choice_b_e;
        dst_p->c.value.b = decoder_read_bool(decoder_p);
        break;

    default:
        decoder_abort(decoder_p, EBADCHOICE);
        break;
    }

    dst_p->d.a = decoder_read_bool(decoder_p);
}

static void uper_my_protocol_b_encode_inner(
    struct encoder_t *encoder_p,
    const struct uper_my_protocol_b_t *src_p)
{
    switch (src_p->choice) {

    case uper_my_protocol_b_choice_a_e:
        encoder_append_non_negative_binary_integer(encoder_p, 0, 1);
        uper_my_protocol_c_encode_inner(encoder_p, &src_p->value.a);
        break;

    case uper_my_protocol_b_choice_b_e:
        encoder_append_non_negative_binary_integer(encoder_p, 1, 1);
        uper_my_protocol_d_encode_inner(encoder_p, &src_p->value.b);
        break;

    default:
        encoder_abort(encoder_p, EBADCHOICE);
        break;
    }
}

static void uper_my_protocol_b_decode_inner(
    struct decoder_t *decoder_p,
    struct uper_my_protocol_b_t *dst_p)
{
    uint8_t choice;

    choice = decoder_read_non_negative_binary_integer(decoder_p, 1);

    switch (choice) {

    case 0:
        dst_p->choice = uper_my_protocol_b_choice_a_e;
        uper_my_protocol_c_decode_inner(decoder_p, &dst_p->value.a);
        break;

    case 1:
        dst_p->choice = uper_my_protocol_b_choice_b_e;
        uper_my_protocol_d_decode_inner(decoder_p, &dst_p->value.b);
        break;

    default:
        decoder_abort(decoder_p, EBADCHOICE);
        break;
    }
}

static void uper_my_protocol_a_encode_inner(
    struct encoder_t *encoder_p,
    const struct uper_my_protocol_a_t *src_p)
{
    uint8_t i;

    encoder_append_non_negative_binary_integer(
        encoder_p,
        src_p->length - 2u,
        2);

    for (i = 0; i < src_p->length; i++) {
        uper_my_protocol_b_encode_inner(encoder_p, &src_p->elements[i]);
    }
}

static void uper_my_protocol_a_decode_inner(
    struct decoder_t *decoder_p,
    struct uper_my_protocol_a_t *dst_p)
{
    uint8_t i;

    dst_p->length = decoder_read_non_negative_binary_integer(
        decoder_p,
        2);
    dst_p->length += 2;

    for (i = 0; i < dst_p->length; i++) {
        uper_my_protocol_b_decode_inner(decoder_p, &dst_p->elements[i]);
    }
}

static void uper_my_protocol_e_encode_inner(
    struct encoder_t *encoder_p,
    const struct uper_my_protocol_e_t *src_p)
{
    encoder_append_bool(encoder_p, src_p->value);
}

static void uper_my_protocol_e_decode_inner(
    struct decoder_t *decoder_p,
    struct uper_my_protocol_e_t *dst_p)
{
    dst_p->value = decoder_read_bool(decoder_p);
}

static void uper_my_protocol_f_encode_inner(
    struct encoder_t *encoder_p,
    const struct uper_my_protocol_f_t *src_p)
{
    encoder_append_int32(encoder_p, src_p->value);
}

static void uper_my_protocol_f_decode_inner(
    struct decoder_t *decoder_p,
    struct uper_my_protocol_f_t *dst_p)
{
    dst_p->value = decoder_read_int32(decoder_p);
}

static void uper_my_protocol_g_encode_inner(
    struct encoder_t *encoder_p,
    const struct uper_my_protocol_g_t *src_p)
{
    encoder_append_uint32(encoder_p, src_p->value);
}

static void uper_my_protocol_g_decode_inner(
    struct decoder_t *decoder_p,
    struct uper_my_protocol_g_t *dst_p)
{
    dst_p->value = decoder_read_uint32(decoder_p);
}

static void uper_my_protocol_pdu_encode_inner(
    struct encoder_t *encoder_p,
    const struct uper_my_protocol_pdu_t *src_p)
{
    encoder_append_int32(encoder_p, src_p->a);

    switch (src_p->b.choice) {

    case uper_my_protocol_pdu_b_choice_a_e:
        encoder_append_non_negative_binary_integer(encoder_p, 0, 3);
        uper_my_protocol_a_encode_inner(encoder_p, &src_p->b.value.a);
        break;

    case uper_my_protocol_pdu_b_choice_b_e:
        encoder_append_non_negative_binary_integer(encoder_p, 1, 3);
        uper_my_protocol_b_encode_inner(encoder_p, &src_p->b.value.b);
        break;

    case uper_my_protocol_pdu_b_choice_c_e:
        encoder_append_non_negative_binary_integer(encoder_p, 2, 3);
        uper_my_protocol_c_encode_inner(encoder_p, &src_p->b.value.c);
        break;

    case uper_my_protocol_pdu_b_choice_d_e:
        encoder_append_non_negative_binary_integer(encoder_p, 3, 3);
        uper_my_protocol_d_encode_inner(encoder_p, &src_p->b.value.d);
        break;

    case uper_my_protocol_pdu_b_choice_e_e:
        encoder_append_non_negative_binary_integer(encoder_p, 4, 3);
        encoder_append_bool(encoder_p, src_p->b.value.e);
        break;

    default:
        encoder_abort(encoder_p, EBADCHOICE);
        break;
    }
}

static void uper_my_protocol_pdu_decode_inner(
    struct decoder_t *decoder_p,
    struct uper_my_protocol_pdu_t *dst_p)
{
    uint8_t choice;

    dst_p->a = decoder_read_int32(decoder_p);
    choice = decoder_read_non_negative_binary_integer(decoder_p, 3);

    switch (choice) {

    case 0:
        dst_p->b.choice = uper_my_protocol_pdu_b_choice_a_e;
        uper_my_protocol_a_decode_inner(decoder_p, &dst_p->b.value.a);
        break;

    case 1:
        dst_p->b.choice = uper_my_protocol_pdu_b_choice_b_e;
        uper_my_protocol_b_decode_inner(decoder_p, &dst_p->b.value.b);
        break;

    case 2:
        dst_p->b.choice = uper_my_protocol_pdu_b_choice_c_e;
        uper_my_protocol_c_decode_inner(decoder_p, &dst_p->b.value.c);
        break;

    case 3:
        dst_p->b.choice = uper_my_protocol_pdu_b_choice_d_e;
        uper_my_protocol_d_decode_inner(decoder_p, &dst_p->b.value.d);
        break;

    case 4:
        dst_p->b.choice = uper_my_protocol_pdu_b_choice_e_e;
        dst_p->b.value.e = decoder_read_bool(decoder_p);
        break;

    default:
        decoder_abort(decoder_p, EBADCHOICE);
        break;
    }
}

ssize_t uper_my_protocol_d_encode(
    uint8_t *dst_p,
    size_t size,
    const struct uper_my_protocol_d_t *src_p)
{
    struct encoder_t encoder;

    encoder_init(&encoder, dst_p, size);
    uper_my_protocol_d_encode_inner(&encoder, src_p);

    return (encoder_get_result(&encoder));
}

ssize_t uper_my_protocol_d_decode(
    struct uper_my_protocol_d_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    struct decoder_t decoder;

    decoder_init(&decoder, src_p, size);
    uper_my_protocol_d_decode_inner(&decoder, dst_p);

    return (decoder_get_result(&decoder));
}

ssize_t uper_my_protocol_c_encode(
    uint8_t *dst_p,
    size_t size,
    const struct uper_my_protocol_c_t *src_p)
{
    struct encoder_t encoder;

    encoder_init(&encoder, dst_p, size);
    uper_my_protocol_c_encode_inner(&encoder, src_p);

    return (encoder_get_result(&encoder));
}

ssize_t uper_my_protocol_c_decode(
    struct uper_my_protocol_c_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    struct decoder_t decoder;

    decoder_init(&decoder, src_p, size);
    uper_my_protocol_c_decode_inner(&decoder, dst_p);

    return (decoder_get_result(&decoder));
}

ssize_t uper_my_protocol_b_encode(
    uint8_t *dst_p,
    size_t size,
    const struct uper_my_protocol_b_t *src_p)
{
    struct encoder_t encoder;

    encoder_init(&encoder, dst_p, size);
    uper_my_protocol_b_encode_inner(&encoder, src_p);

    return (encoder_get_result(&encoder));
}

ssize_t uper_my_protocol_b_decode(
    struct uper_my_protocol_b_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    struct decoder_t decoder;

    decoder_init(&decoder, src_p, size);
    uper_my_protocol_b_decode_inner(&decoder, dst_p);

    return (decoder_get_result(&decoder));
}

ssize_t uper_my_protocol_a_encode(
    uint8_t *dst_p,
    size_t size,
    const struct uper_my_protocol_a_t *src_p)
{
    struct encoder_t encoder;

    encoder_init(&encoder, dst_p, size);
    uper_my_protocol_a_encode_inner(&encoder, src_p);

    return (encoder_get_result(&encoder));
}

ssize_t uper_my_protocol_a_decode(
    struct uper_my_protocol_a_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    struct decoder_t decoder;

    decoder_init(&decoder, src_p, size);
    uper_my_protocol_a_decode_inner(&decoder, dst_p);

    return (decoder_get_result(&decoder));
}

ssize_t uper_my_protocol_e_encode(
    uint8_t *dst_p,
    size_t size,
    const struct uper_my_protocol_e_t *src_p)
{
    struct encoder_t encoder;

    encoder_init(&encoder, dst_p, size);
    uper_my_protocol_e_encode_inner(&encoder, src_p);

    return (encoder_get_result(&encoder));
}

ssize_t uper_my_protocol_e_decode(
    struct uper_my_protocol_e_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    struct decoder_t decoder;

    decoder_init(&decoder, src_p, size);
    uper_my_protocol_e_decode_inner(&decoder, dst_p);

    return (decoder_get_result(&decoder));
}

ssize_t uper_my_protocol_f_encode(
    uint8_t *dst_p,
    size_t size,
    const struct uper_my_protocol_f_t *src_p)
{
    struct encoder_t encoder;

    encoder_init(&encoder, dst_p, size);
    uper_my_protocol_f_encode_inner(&encoder, src_p);

    return (encoder_get_result(&encoder));
}

ssize_t uper_my_protocol_f_decode(
    struct uper_my_protocol_f_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    struct decoder_t decoder;

    decoder_init(&decoder, src_p, size);
    uper_my_protocol_f_decode_inner(&decoder, dst_p);

    return (decoder_get_result(&decoder));
}

ssize_t uper_my_protocol_g_encode(
    uint8_t *dst_p,
    size_t size,
    const struct uper_my_protocol_g_t *src_p)
{
    struct encoder_t encoder;

    encoder_init(&encoder, dst_p, size);
    uper_my_protocol_g_encode_inner(&encoder, src_p);

    return (encoder_get_result(&encoder));
}

ssize_t uper_my_protocol_g_decode(
    struct uper_my_protocol_g_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    struct decoder_t decoder;

    decoder_init(&decoder, src_p, size);
    uper_my_protocol_g_decode_inner(&decoder, dst_p);

    return (decoder_get_result(&decoder));
}

ssize_t uper_my_protocol_pdu_encode(
    uint8_t *dst_p,
    size_t size,
    const struct uper_my_protocol_pdu_t *src_p)
{
    struct encoder_t encoder;

    encoder_init(&encoder, dst_p, size);
    uper_my_protocol_pdu_encode_inner(&encoder, src_p);

    return (encoder_get_result(&encoder));
}

ssize_t uper_my_protocol_pdu_decode(
    struct uper_my_protocol_pdu_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    struct decoder_t decoder;

    decoder_init(&decoder, src_p, size);
    uper_my_protocol_pdu_decode_inner(&decoder, dst_p);

    return (decoder_get_result(&decoder));
}

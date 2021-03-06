EXPECTED = {'InformationObject': {'extensibility-implied': False,
                       'imports': {'InformationObjectClass': ['ITEM']},
                       'object-classes': {'A': {'members': [{'name': '&a',
                                                             'type': 'INTEGER'},
                                                            {'name': '&B',
                                                             'type': 'OpenType'}]},
                                          'ERROR-CLASS': {'members': [{'name': '&category',
                                                                       'type': 'PrintableString'},
                                                                      {'name': '&code',
                                                                       'type': 'INTEGER'},
                                                                      {'name': '&Type',
                                                                       'type': 'OpenType'}]},
                                          'GC': {'members': [{'name': '&id',
                                                              'type': 'INTEGER'},
                                                             {'name': '&Value',
                                                              'type': 'OpenType'}]},
                                          'INNER-ITEM': {'members': [{'name': '&type',
                                                                      'type': 'INTEGER'},
                                                                     {'name': '&Value',
                                                                      'type': 'OpenType'}]}},
                       'object-sets': {'As': {'class': 'A',
                                              'members': [{'&B': {'type': 'C'},
                                                           '&a': 0}]},
                                       'ErrorSet': {'class': 'ERROR-CLASS',
                                                    'members': [{'&Type': {'type': 'INTEGER'},
                                                                 '&category': 'A',
                                                                 '&code': 1},
                                                                {'&Type': {'type': 'BOOLEAN'},
                                                                 '&category': 'A',
                                                                 '&code': 2},
                                                                {'&Type': {'type': 'IA5String'},
                                                                 '&category': 'B',
                                                                 '&code': 1},
                                                                {'&Type': {'type': 'GeneralString'},
                                                                 '&category': 'B',
                                                                 '&code': 2}]},
                                       'GCs': {'class': 'GC', 'members': []},
                                       'InnerItems': {'class': 'INNER-ITEM',
                                                      'members': ['innerItem0',
                                                                  {'&Value': {'type': 'IA5String'},
                                                                   '&type': 1}]},
                                       'Items': {'class': 'ITEM',
                                                 'members': [{'&Value': {'type': 'BOOLEAN'},
                                                              '&comment': 'item '
                                                                          '0',
                                                              '&id': 0},
                                                             {'&Value': {'type': 'InnerSequence'},
                                                              '&comment': 'item '
                                                                          '1',
                                                              '&id': 1}]}},
                       'tags': 'AUTOMATIC',
                       'types': {'C': {'members': [{'name': 'a',
                                                    'table': {'type': 'As'},
                                                    'type': 'A.&a'},
                                                   {'name': 'b',
                                                    'optional': True,
                                                    'table': ['As', ['a']],
                                                    'type': 'A.&B'}],
                                       'type': 'SEQUENCE'},
                                 'D1': {'members': [{'name': 'id',
                                                     'table': {'type': 'GCs'},
                                                     'type': 'GC.&id'},
                                                    {'name': 'value',
                                                     'table': ['GCs', ['id']],
                                                     'type': 'GC.&Value'},
                                                    {'actual-parameters': ['{'],
                                                     'name': 'a',
                                                     'type': 'E1'}],
                                        'type': 'SEQUENCE'},
                                 'D2': {'members': [{'name': 'id',
                                                     'table': {'type': 'GCs'},
                                                     'type': 'GC.&id'},
                                                    {'name': 'value',
                                                     'table': ['GCs', ['id']],
                                                     'type': 'GC.&Value'},
                                                    {'name': 'a',
                                                     'type': 'T1'}],
                                        'type': 'SEQUENCE'},
                                 'E1': {'members': [{'name': 'id',
                                                     'table': {'type': 'Param'},
                                                     'type': 'GC.&id'},
                                                    {'name': 'value',
                                                     'table': ['Param', ['id']],
                                                     'type': 'GC.&Value'}],
                                        'parameters': ['Param'],
                                        'type': 'SEQUENCE'},
                                 'ErrorReturn': {'members': [{'name': 'errorCategory',
                                                              'table': {'type': 'ErrorSet'},
                                                              'type': 'ERROR-CLASS.&category'},
                                                             {'element': {'members': [{'name': 'errorCode',
                                                                                       'table': ['ErrorSet',
                                                                                                 ['errorCategory']],
                                                                                       'type': 'ERROR-CLASS.&code'},
                                                                                      {'name': 'errorInfo',
                                                                                       'table': ['ErrorSet',
                                                                                                 ['errorCategory',
                                                                                                  '.errorCode']],
                                                                                       'type': 'ERROR-CLASS.&Type'}],
                                                                          'type': 'SEQUENCE'},
                                                              'name': 'errors',
                                                              'type': 'SEQUENCE '
                                                                      'OF'}],
                                                 'type': 'SEQUENCE'},
                                 'InnerSequence': {'members': [{'name': 'myValue',
                                                                'table': ['InnerItems',
                                                                          ['myType']],
                                                                'type': 'INNER-ITEM.&Value'},
                                                               {'name': 'myType',
                                                                'table': {'type': 'InnerItems'},
                                                                'type': 'INNER-ITEM.&type'}],
                                                   'type': 'SEQUENCE'},
                                 'ItemWithConstraints': {'members': [{'name': 'id',
                                                                      'table': {'type': 'Items'},
                                                                      'type': 'ITEM.&id'},
                                                                     {'name': 'value',
                                                                      'table': ['Items',
                                                                                ['id']],
                                                                      'type': 'ITEM.&Value'},
                                                                     {'name': 'comment',
                                                                      'table': ['Items',
                                                                                ['id']],
                                                                      'type': 'ITEM.&comment'},
                                                                     {'name': 'extra',
                                                                      'type': 'INTEGER'}],
                                                         'type': 'SEQUENCE'},
                                 'ItemWithoutConstraints': {'members': [{'name': 'id',
                                                                         'table': None,
                                                                         'type': 'ITEM.&id'},
                                                                        {'name': 'value',
                                                                         'table': None,
                                                                         'type': 'ITEM.&Value'},
                                                                        {'name': 'comment',
                                                                         'table': None,
                                                                         'type': 'ITEM.&comment'},
                                                                        {'name': 'extra',
                                                                         'type': 'INTEGER'}],
                                                            'type': 'SEQUENCE'},
                                 'String': {'type': 'NULL'},
                                 'T1': {'members': [{'name': 'id',
                                                     'table': {'type': 'GCs'},
                                                     'type': 'GC.&id'},
                                                    {'name': 'value',
                                                     'table': ['GCs', ['id']],
                                                     'type': 'GC.&Value'}],
                                        'type': 'SEQUENCE'}},
                       'values': {'innerItem0': {'type': 'INNER-ITEM',
                                                 'value': None}}},
 'InformationObjectClass': {'extensibility-implied': False,
                            'imports': {},
                            'object-classes': {'ITEM': {'members': [{'name': '&id',
                                                                     'type': 'INTEGER'},
                                                                    {'name': '&Value',
                                                                     'type': 'OpenType'},
                                                                    {'name': '&comment',
                                                                     'type': 'String'}]}},
                            'object-sets': {},
                            'tags': 'AUTOMATIC',
                            'types': {'String': {'type': 'IA5String'}},
                            'values': {}}}
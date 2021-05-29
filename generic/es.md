## Admin

### Health

```
$ curl 'http://localhost:9200/_cat/health?v'
epoch      timestamp cluster       status node.total node.data shards pri relo init unassign pending_tasks max_task_wait_time active_shards_percent
1564744336 11:12:16  elasticsearch yellow          1         1      6   6    0    0        1             0                  -                 85.7%
```

### List Indices

```
$ curl 'http://localhost:9200/_cat/indices?v'
health status index                        uuid                   pri rep docs.count docs.deleted store.size pri.store.size
green  open   .kibana_task_manager         KRJo5uusSuSRgBbJI6uTnw   1   0          2            0     12.7kb         12.7kb
yellow open   test                         r9jaZWlzSf6bu2brYe-g8w   1   1          3            1     11.9kb         11.9kb
green  open   kibana_sample_data_ecommerce wHfpVHzJRMSTeM8sKt3WWQ   1   0       4675            0      4.8mb          4.8mb
green  open   kibana_sample_data_logs      YzLUIqe_QEW1DM459txyUw   1   0      14074            0     11.8mb         11.8mb
green  open   .kibana_1                    wG_TRqtbSG2m99WoZf59_Q   1   0        152            4        1mb            1mb
green  open   kibana_sample_data_flights   p0xKbmQ8TfKTtijXXiqtPQ   1   0      13059            0      6.6mb          6.6mb
```

### Create Index

```
$ curl -X PUT http://localhost:9200/customer?pretty
{
  "acknowledged" : true,
  "shards_acknowledged" : true,
  "index" : "customer"
}
```

### Delete Index

```
$ curl -X DELETE http://localhost:9200/customer?pretty 
{
  "acknowledged" : true
}
$ curl http://localhost:9200/_cat/indices?v
health status index                        uuid                   pri rep docs.count docs.deleted store.size pri.store.size
green  open   .kibana_task_manager         KRJo5uusSuSRgBbJI6uTnw   1   0          2            0     12.7kb         12.7kb
yellow open   test                         r9jaZWlzSf6bu2brYe-g8w   1   1          3            1     11.9kb         11.9kb
green  open   kibana_sample_data_ecommerce wHfpVHzJRMSTeM8sKt3WWQ   1   0       4675            0      4.8mb          4.8mb
green  open   kibana_sample_data_logs      YzLUIqe_QEW1DM459txyUw   1   0      14074            0     11.8mb         11.8mb
green  open   .kibana_1                    wG_TRqtbSG2m99WoZf59_Q   1   0        152            4        1mb            1mb
green  open   kibana_sample_data_flights   p0xKbmQ8TfKTtijXXiqtPQ   1   0      13059            0      6.6mb          6.6mb
```

## Basic 

### Insert Document

```
$ curl -X PUT -H 'Content-Type: application/json' http://localhost:9200/customer/_doc/1?pretty -d '{"name": "John Doe"}'
{
  "_index" : "customer",
  "_type" : "_doc",
  "_id" : "1",
  "_version" : 2,
  "result" : "updated",
  "_shards" : {
    "total" : 2,
    "successful" : 1,
    "failed" : 0
  },
  "_seq_no" : 1,
  "_primary_term" : 1
}
```

### Get by id

```
$ curl http://localhost:9200/customer/_doc/1?pretty
{
  "_index" : "customer",
  "_type" : "_doc",
  "_id" : "1",
  "_version" : 2,
  "_seq_no" : 1,
  "_primary_term" : 1,
  "found" : true,
  "_source" : {
    "name" : "John Doe"
  }
}
```

### Insert Document without ID

```
# curl -X POST -H 'Content-Type: application/json' http://localhost:9200/customer/_doc/?pretty -d '{"name": "Hoho"}'
{
  "_index" : "customer",
  "_type" : "_doc",
  "_id" : "pl8eUmwB_5GtPQqoF8tm",                   # <====
  "_version" : 1,
  "result" : "created",
  "_shards" : {
    "total" : 2,
    "successful" : 1,
    "failed" : 0
  },
  "_seq_no" : 1,
  "_primary_term" : 1
}
```

### Update Docuement

```
$ curl -X POST -H 'Content-Type: application/json' http://localhost:9200/customer/_update/1?pretty -d '{ "doc": {"name": "Jane Doe", "age": 20} }'
{
  "_index" : "customer",
  "_type" : "_doc",
  "_id" : "1",
  "_version" : 2,
  "result" : "updated",
  "_shards" : {
    "total" : 2,
    "successful" : 1,
    "failed" : 0
  },
  "_seq_no" : 2,
  "_primary_term" : 1
}
```

### Update Document (script)

$ curl -X POST -H 'Content-Type: application/json' http://localhost:9200/customer/_update/1?pretty -d '{ "script": "ctx._source.age += 5" }'
{
  "_index" : "customer",
  "_type" : "_doc",
  "_id" : "1",
  "_version" : 3,
  "result" : "updated",
  "_shards" : {
    "total" : 2,
    "successful" : 1,
    "failed" : 0
  },
  "_seq_no" : 3,
  "_primary_term" : 1
}

### Delete Doucment

```
$ curl -X DELETE http://localhost:9200/customer/_doc/pl8eUmwB_5GtPQqoF8tm?pretty
{
  "_index" : "customer",
  "_type" : "_doc",
  "_id" : "pl8eUmwB_5GtPQqoF8tm",
  "_version" : 2,
  "result" : "deleted",
  "_shards" : {
    "total" : 2,
    "successful" : 1,
    "failed" : 0
  },
  "_seq_no" : 5,
  "_primary_term" : 1
}
```

### Batch

```
# curl -X POST "localhost:9200/customer/_bulk?pretty" -H 'Content-Type: application/json' -d'
{"index":{"_id":"1"}}
{"name": "John Doe" }
{"index":{"_id":"2"}}
{"name": "Jane Doe" }
'
````

### Batch 2

```
# curl -X POST "localhost:9200/customer/_bulk?pretty" -H 'Content-Type: application/json' -d'
{"update":{"_id":"1"}}
{"doc": { "name": "John Doe becomes Jane Doe" } }
{"delete":{"_id":"2"}}
'
```

## Search


### Basic

```
curl -X GET "localhost:9200/bank/_search?q=*&sort=account_number:asc&pretty"
```

or 

```
curl -X GET "localhost:9200/bank/_search" -H 'Content-Type: application/json' -d'
{
  "query": { "match_all": {} },
  "sort": [
    { "account_number": "asc" }
  ]
}
'
```

### Search (form/size)

```
curl -X GET "localhost:9200/bank/_search" -H 'Content-Type: application/json' -d'
{
  "query": { "match_all": {} },
  "sort": [
    { "account_number": "asc" }
  ],
  "from": 10,
  "size": 2
}
'
```

### Search (Select fields)

```
curl -X GET "localhost:9200/bank/_search" -H 'Content-Type: application/json' -d'
{
  "query": { "match_all": {} },
  "_source": ["account_number", "balance"]
}
'
```

### Search (Match)

mill or lane

```
curl -X GET "localhost:9200/bank/_search" -H 'Content-Type: application/json' -d'
{
  "query": { "match": { "address": "mill lane" } }
}
'
```

excat match

```
curl -X GET "localhost:9200/bank/_search" -H 'Content-Type: application/json' -d'
{
  "query": { "match_phrase": { "address": "mill lane" } }
}
'
```

### Search (bool)

logical AND

```
curl -X GET "localhost:9200/bank/_search" -H 'Content-Type: application/json' -d'
{
  "query": {
    "bool": {
      "must": [
        { "match": { "address": "mill" } },
        { "match": { "address": "lane" } }
      ]
    }
  }
}
'

```

logical OR 

```
curl -X GET "localhost:9200/bank/_search" -H 'Content-Type: application/json' -d'
{
  "query": {
    "bool": {
      "should": [
        { "match": { "address": "mill" } },
        { "match": { "address": "lane" } }
      ]
    }
  }
}
'
```

logical NOT

```
curl -X GET "localhost:9200/bank/_search" -H 'Content-Type: application/json' -d'
{
  "query": {
    "bool": {
      "must_not": [
        { "match": { "address": "mill" } },
        { "match": { "address": "lane" } }
      ]
    }
  }
}
'
```


combined

```
curl -X GET "localhost:9200/bank/_search" -H 'Content-Type: application/json' -d'
{
  "query": {
    "bool": {
      "must": [
        { "match": { "age": "40" } }
      ],
      "must_not": [
        { "match": { "state": "ID" } }
      ]
    }
  }
}
'
```

### Search (Filter)

```
curl -X GET "localhost:9200/bank/_search" -H 'Content-Type: application/json' -d'
{
  "query": {
    "bool": {
      "must": { "match_all": {} },
      "filter": {
        "range": {
          "balance": {
            "gte": 20000,
            "lte": 30000
          }
        }
      }
    }
  }
}
'
```


### Aggs GroupBy 

```
curl -X GET "localhost:9200/bank/_search" -H 'Content-Type: application/json' -d'
{
  "size": 0,
  "aggs": {
    "group_by_state": {
      "terms": {
        "field": "state.keyword"
      }
    }
  }
}
'
````

### Aggs Avg

```
curl -X GET "localhost:9200/bank/_search" -H 'Content-Type: application/json' -d'
{
  "size": 0,
  "aggs": {
    "group_by_state": {
      "terms": {
        "field": "state.keyword"
      },
      "aggs": {
        "average_balance": {
          "avg": {
            "field": "balance"
          }
        }
      }
    }
  }
}
'
```

### Aggs Range

```
curl -X GET "localhost:9200/bank/_search" -H 'Content-Type: application/json' -d'
{
  "size": 0,
  "aggs": {
    "group_by_age": {
      "range": {
        "field": "age",
        "ranges": [
          {
            "from": 20,
            "to": 30
          },
          {
            "from": 30,
            "to": 40
          },
          {
            "from": 40,
            "to": 50
          }
        ]
      },
      "aggs": {
        "group_by_gender": {
          "terms": {
            "field": "gender.keyword"
          },
          "aggs": {
            "average_balance": {
              "avg": {
                "field": "balance"
              }
            }
          }
        }
      }
    }
  }
}
'
```
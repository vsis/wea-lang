# WEA LANG

They say chileans use _wea_ for everything. So, this is a language that uses `wea` for everything.


## Run tests

```
make tests
```

## Run tests with docker

```
docker build -t wea-test -f test.dockerfile .
docker run --rm -ti -v $(pwd)/src:/home/wea/src wea-test make tests
```


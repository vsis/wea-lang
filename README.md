# WEA LANG

They say chileans use _wea_ for everything. So, this is a language that uses `wea` for everything.


## Run tests

```
make check
```

## Run tests with docker

```
docker build -t vsis/wea-test -f test.dockerfile .
docker run --rm -ti -v $(pwd)/src:/home/wea/src vsis/wea-test
```


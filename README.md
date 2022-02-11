# WEA LANG

[![pipeline](https://gitlab.com/roddy.gonzalez.89/wea-lang/badges/master/pipeline.svg)](https://gitlab.com/roddy.gonzalez.89/wea-lang/-/commits/master)
[![coverage](https://gitlab.com/roddy.gonzalez.89/wea-lang/badges/master/coverage.svg)](https://gitlab.com/roddy.gonzalez.89/wea-lang/-/commits/master)

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


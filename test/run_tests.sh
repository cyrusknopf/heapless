RED='\033[0;31m'
GREEN='\033[0;36m'
NOCOL='\033[0m'

for testdir in test/*/; do
  RES=`diff <($CC -fplugin=./$PLUGIN -c "$testdir"test.c 2>&1) "$testdir"expected.txt;`
  if [[ -z "$RES" ]]; then
    echo -e "${GREEN}$testdir" passed${NOCOL};
  else
    echo -e "${RED}$testdir" failed${NOCOL};
  fi
done


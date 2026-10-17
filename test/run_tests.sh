RED='\033[0;31m'
CYAN='\033[0;36m'
YELLOW='\033[0;33m'
NOCOL='\033[0m'

for testdir in test/*/; do
  [[ -e "$testdir"test.c ]] && CMP="$CC" || CMP="$CXX";
  [[ -e "$testdir"test.c ]] && SRC="test.c" || SRC="test.cpp";

  if [[ ! -e "$testdir""$SRC" ]]; then
    echo -e "${YELLOW}$testdir$SRC not found${NOCOl}";
    exit -1;
  fi

  RES=`diff <($CMP -fplugin=./$PLUGIN -c "$testdir""$SRC" 2>&1) "$testdir"expected.txt;`
  if [[ -z "$RES" ]]; then
    echo -e "${CYAN}$testdir" passed${NOCOL};
  else
    echo -e "${RED}$testdir" failed${NOCOL};
    $CC -fplugin=./$PLUGIN -c "$testdir""$SRC" 2>&1
  fi
done


flex -Caf -v Filler.lex
sed -i '' 's/#include <FlexLexer.h>/#include \"waspcore\/FlexLexer.h\"/' FillerLexer.cpp 
bison --version
bison Filler.bison --report=state --report=lookahead
sed -i '' 's/class FillerParser/class WASP_PUBLIC FillerParser/' FillerParser.hpp
#bison Filler.bison --report=state --report=lookahead --report=solved  -g

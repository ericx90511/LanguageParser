CXX_DIR = ../lib/cxxtest
CXXTEST = $(CXX_DIR)/bin/cxxtestgen
CXXFLAGS = --error-printer --abort-on-fail --have-eh

FLAGS = -Wall -g

# Program files.
readInput.o:	readInput.cpp readInput.h
	g++ $(FLAGS) -c readInput.cpp 

regex.o:	regex.cpp regex.h
	g++ $(FLAGS) -c regex.cpp 

scanner.o:	scanner.cpp scanner.h regex.h
	g++ $(FLAGS) -c scanner.cpp

ExtToken.o: ExtToken.cpp ExtToken.h Parser.h
	g++ $(FLAGS) -c ExtToken.cpp

Parser.o: Parser.cpp Parser.h scanner.h ExtToken.h AST.h
	g++ $(FLAGS) -c Parser.cpp

AST.o: AST.cpp AST.h 
	g++ $(FLAGS) -c AST.cpp

ParseResult.o: ParseResult.cpp ParseResult.h
	g++ $(FLAGS) -c ParseResult.cpp

# Testing files and targets.
run-tests:	regex_tests scanner_tests i1_assessment_tests i2_assessment_tests i3_assessment_tests parser_tests i4_assessment_tests
	./regex_tests
	./i1_assessment_tests
	./scanner_tests
	./i2_assessment_tests
	./parser_tests
	./i3_assessment_tests
	./i4_assessment_tests

derp: parser_tests
	./parser_tests

regex_tests:	regex_tests.cpp regex.o
	g++ $(FLAGS) -I$(CXX_DIR) -o regex_tests regex.o regex_tests.cpp

regex_tests.cpp:	regex.h regex_tests.h
	$(CXXTEST) $(CXXFLAGS) -o regex_tests.cpp regex_tests.h

scanner_tests:	scanner_tests.cpp scanner.o regex.o readInput.o
	g++ $(FLAGS) -I$(CXX_DIR)  -o scanner_tests \
		scanner.o regex.o readInput.o scanner_tests.cpp

scanner_tests.cpp:	scanner.o scanner_tests.h readInput.h
	$(CXXTEST) $(CXXFLAGS) -o scanner_tests.cpp scanner_tests.h

parser_tests: parser_tests.cpp scanner.o readInput.o ExtToken.o regex.o Parser.o AST.o ParseResult.o
	g++ $(FLAGS) -I$(CXX_DIR) -o parser_tests\
		scanner.o readInput.o parser_tests.cpp ExtToken.o regex.o Parser.o AST.o ParseResult.o

parser_tests.cpp: parser_tests.h readInput.h scanner.h Parser.h ExtToken.h
	$(CXXTEST) $(CXXFLAGS) -o parser_tests.cpp parser_tests.h

i1_assessment_tests:	i1_assessment_tests.cpp scanner.o regex.o readInput.o
	g++ $(FLAGS) -I$(CXX_DIR)  -o i1_assessment_tests \
		scanner.o regex.o readInput.o i1_assessment_tests.cpp

i1_assessment_tests.cpp:	scanner.o i1_assessment_tests.h readInput.h
	$(CXXTEST) $(CXXFLAGS) -o i1_assessment_tests.cpp i1_assessment_tests.h

i2_assessment_tests: i2_assessment_tests.cpp regex.o readInput.o scanner.o ExtToken.o Parser.o ParseResult.o AST.o
	g++ $(FLAGS) -I$(CXX_DIR) -o i2_assessment_tests \
		readInput.o scanner.o regex.o ExtToken.o Parser.o ParseResult.o AST.o i2_assessment_tests.cpp

i2_assessment_tests.cpp: readInput.h scanner.o ExtToken.h Parser.h ParseResult.h 
	$(CXXTEST) $(CXXFLAGS) -o i2_assessment_tests.cpp i2_assessment_tests.h

i3_assessment_tests: i3_assessment_tests.cpp readInput.o scanner.o ExtToken.o AST.o Parser.o ParseResult.o regex.o
	g++ $(FLAGS) -I$(CXX_DIR) -o i3_assessment_tests \
		readInput.o scanner.o regex.o ExtToken.o Parser.o ParseResult.o AST.o i3_assessment_tests.cpp

i3_assessment_tests.cpp: readInput.h scanner.o ExtToken.h Parser.h ParseResult.h
	$(CXXTEST) $(CXXFLAGS) -o i3_assessment_tests.cpp i3_assessment_tests.h

i4_assessment_tests: i4_assessment_tests.cpp readInput.o scanner.o ExtToken.o AST.o Parser.o ParseResult.o regex.o
	g++ $(FLAGS) -I$(CXX_DIR) -o i4_assessment_tests \
		readInput.o scanner.o regex.o ExtToken.o Parser.o ParseResult.o AST.o i4_assessment_tests.cpp

i4_assessment_tests.cpp: readInput.h scanner.o ExtToken.h Parser.h ParseResult.h
	$(CXXTEST) $(CXXFLAGS) -o i4_assessment_tests.cpp i4_assessment_tests.h

clean:
	rm -Rf *.o \
		regex_tests regex_tests.cpp \
		scanner_tests scanner_tests.cpp \
		parser_tests parser_tests.cpp\
		i1_assessment_tests i1_assessment_tests.cpp \
		i2_assessment_tests i2_assessment_tests.cpp\
		i3_assessment_tests i3_assessment_tests.cpp\
		i4_assessment_tests i4_assessment_tests.cpp\
		a.out  

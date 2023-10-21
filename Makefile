#Makefile先看变量设置了什么
CFLAGS=-g -O2 -Wall -I src/ -Wextra -rdynamic -DNDEBUG $(OPTFLAGS) 
LIBS=-ldl $(OPTLIBS)
PREFIX?=/usr/local

SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))					#有这个变量，但是还没有.o文件

TEST_SRC=$(wildcard tests/*_tests.c)
TESTS=$(patsubst %.c,%,$(TEST_SRC))						#将.c去掉是为了利用make自动识别目标名使用gcc进行编译

TARGET=build/liblcthdw.a
SO_TARGET=$(patsubst %.a,%.so,$(TARGET))

# The Target Build
all: $(TARGET) $(SO_TARGET) tests						#这里的test是非典型的target

dev: CFLAGS=-g -Wall -I /src -Wall -Wextra $(OPTFLAGS)	#开发构建模式
dev: all

$(TARGET): CFLAGS += -fPIC		#添加-fPIC到CFLAGS
$(TARGET): build $(OBJECTS)		#构建，target为build和objects组的规则需要被关注
	ar rcs $@ $(OBJECTS)		#生成.a文件
	ranlib $@

$(SO_TARGET): $(TARGET) $(OBJECTS)		#构建.so,target为target和objects的规则或者变量需要被关注
	$(CC) -shared -o $@ $(OBJECTS)		#'@'符号可联想为邮箱，一个目的地(target). 生成.so文件

build:
	@mkdir -p build
	@mkdir -p bin

# The Unit Tests
.PHONY: tests
tests: $(TESTS) 
	sh ./tests/runtests.sh

$(TESTS): $(TEST_SRC)
	$(CC) -g -O2 -Wall -I src/ -Wextra -rdynamic $@.c -o $@ $(TARGET)		#$(TESTS_SRC)展开将会是两个文件而重复定义一些变量
	
valgrind:
VALGRIND="valgrind --log-file=/tmp/valgrind-%p.log" $(MAKE)

# The Cleaner
clean:
	rm -rf build $(OBJECTS) $(TESTS)
	rm -f tests/tests.log 
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`

# The Install
install: all
	install -d $(DESTDIR)/$(PREFIX)/lib/
	install $(TARGET) $(DESTDIR)/$(PREFIX)/lib/

# The Checker
check:
	@echo Files with potentially dangerous functions.
	@egrep '[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)\
		|stpn?cpy|a?sn?printf|byte_)' $(SOURCES) || true


CFLAGS = -msse2 --std gnu99 -O0 -Wall -Wextra

EXEC = naive_transpose sse_transpose sse_prefetch_transpose

GIT_HOOKS := .git/hooks/applied

SRCS_common = main.c

all: $(GIT_HOOKS) $(EXEC)

naive_transpose: $(SRCS_common) 
	$(CC) $(CFLAGS) -o $@ \
	-DNAIVE  \
	$(SRCS_common) 

sse_transpose: $(SRCS_common)
	$(CC) $(CFLAGS) -o $@ \
	-DSSE \
	$(SRCS_common)


sse_prefetch_transpose: $(SRCS_common)
	$(CC) $(CFLAGS) -o $@ \
	-DSSE_PREFETCH \
	$(SRCS_common)

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

clean:
	$(RM) $(EXEC)

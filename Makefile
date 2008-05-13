CC=sdcc
GFLAGS=-mz80 --nooverlay
CFLAGS=$(GFLAGS) --nostdlib --nostdinc --stack-auto -I./

AS=as-z80
ASFLAGS=-plosgff

AR=ar
ARFLAGS=-cr

CODE_ADDR=0x0000
DATA_ADDR=0x4000
STACK_ADDR=0xffff
# Resulting image file will be this many bytes
IMAGE_SIZE=$(shell printf %d $(DATA_ADDR))

LD=$(CC)
LDFLAGS=$(GFLAGS) --no-std-crt0 --code-loc $(CODE_ADDR) --data-loc $(DATA_ADDR) \
 --stack-loc $(STACK_ADDR)

# The hex2bin util used to convert our ihx file to a binary
HEX2BIN=hex2bin
H2BFLAGS=-s $(CODE_ADDR)

.SUFFIXES: .asm .bin .c .d .ihx .o

# Compile C files into object files, taking dependencies into account.
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<
	rm -f $(@:.o=.asm) $(@:.o=.lst) $(@:.o=.sym)

# Assemble assmebly code into object files.
%.o: %.s
	$(AS) $(ASFLAGS) -o $@ $<
	rm -f $(@:.o=.lst) $(@:.o=.sym) $(@:.o=.lnk)

# Make a dependency file for a C source file (these are included below).
# The regex is used to reformat sdcc's output into something we can use.
%.d: %.c
	$(CC) $(CFLAGS) -M $< | sed "s/$*.rel: $*.c/$*.o:/" > $@

SRCS=$(shell ls *.c 2> /dev/null)
COBJS=$(SRCS:.c=.o)
DEPS=$(SRCS:.c=.d)
CJUNK=$(SRCS:.c=.asm) $(SRCS:.c=.lst) $(SRCS:.c=.sym)
# boot.s must come first here
ASMSRCS=boot.s asm_util.s
ASMOBJS=$(ASMSRCS:.s=.o)
ASMJUNK=$(ASMSRCS:.s=.lst) $(ASMSRCS:.s=.sym) $(ASMSRCS:.s=.lnk)

OBJS=$(ASMOBJS) $(COBJS)

all: monitor.bin

clean:
	rm -f $(OBJS) $(ASMJUNK) $(CJUNK) $(DEPS) monitor.*

dep: $(DEPS)

monitor.bin: monitor.tmp
#	dd if=$< of=$@ bs=$(IMAGE_SIZE) count=1 conv=sync
	cp monitor.tmp monitor.bin

monitor.tmp: monitor.ihx
	$(HEX2BIN) $(H2BFLAGS) -e tmp $< | grep 'address'

monitor.ihx: $(OBJS)
	$(LD) $(LDFLAGS) -o monitor $^

# Makefile for pxml - version 1.0.0 - experimental[240628.02-100e]

# File
TARGET = pxml
SOURCE = pxml.c
CONFIG = pxml.xml
MANPAGE = pxml.1

# Library
LDFLAGS = -I/usr/include/libxml2 -lxml2

# Path
BINDIR = /usr/local/bin
MANDIR = /usr/local/man/man1
CONFDIR = /etc


# Rules
all: compile

compile: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) -o $(TARGET) $(SOURCE) $(LDFLAGS)

install: $(TARGET) $(MANPAGE) $(CONFIG)
	@install -d $(BINDIR)
	@install -d $(MANDIR)
	@install -d $(CONFDIR)
	@if [ -e $(BINDIR)/$(TARGET) ]; then \
		echo "$(TARGET) already exists in $(BINDIR). Installation aborted."; \
	else \
		ln -s -r ./$(TARGET) $(BINDIR)/$(TARGET); \
		echo "Installed $(TARGET) to $(BINDIR)"; \
	fi
	@if [ -e $(MANDIR)/$(MANPAGE) ]; then \
		echo "$(MANPAGE) already exists in $(MANDIR). Installation aborted."; \
	else \
		ln -s -r ./$(MANPAGE) $(MANDIR)/$(MANPAGE); \
		echo "Installed $(MANPAGE) to $(MANDIR)"; \
		mandb; \
	fi
	@if [ -e $(CONFDIR)/$(CONFIG) ]; then \
		echo "$(CONFIG) already exists in $(CONFDIR). Installation aborted."; \
	else \
		ln -s -r ./$(CONFIG) $(CONFDIR)/$(CONFIG); \
		echo "Installed $(CONFIG) to $(CONFDIR)"; \
	fi
	@chmod 644 $(MANDIR)/$(MANPAGE)
	@chmod 755 $(BINDIR)/$(TARGET)
	@chmod 644 $(CONFDIR)/$(CONFIG)

uninstall:
	@if [ -L $(BINDIR)/$(TARGET) ]; then \
		rm $(BINDIR)/$(TARGET); \
		echo "Removed $(TARGET) from $(BINDIR)"; \
	else \
		echo "$(TARGET) not found in $(BINDIR). Uninstallation aborted."; \
	fi
	@if [ -L $(MANDIR)/$(MANPAGE) ]; then \
		rm $(MANDIR)/$(MANPAGE); \
		echo "Removed $(MANPAGE) from $(MANDIR)"; \
	else \
		echo "$(MANPAGE) not found in $(MANDIR). Uninstallation aborted."; \
	fi
	@if [ -L $(CONFDIR)/$(CONFIG) ]; then \
		rm $(CONFDIR)/$(CONFIG); \
		echo "Removed $(CONFIG) from $(CONFDIR)"; \
	else \
		echo "$(CONFIG) not found in $(CONFDIR). Uninstallation aborted."; \
	fi
	@mandb

clean:
	rm -f $(TARGET)
	@if [ -e $(TARGET).o ]; then\
		rm -r $(TARGET).o; \
	fi

.PHONY: all compile install uninstall clean


include $(GOROOT)/src/Make.$(GOARCH)

PKGDIR=$(GOROOT)/pkg/$(GOOS)_$(GOARCH)

TARG=trainers
CGOFILES=cBruteTreeTrainer.go

include $(GOROOT)/src/Make.pkg

CLEANFILES+=main $(PKGDIR)/$(TARG).a

main: install main.go
	  $(GC) main.go
	  $(LD) -o $@ main.$O

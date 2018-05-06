FILE = P0860.bs
HTML = P0860r1.html

all:
	curl -k https://api.csswg.org/bikeshed/ -F file=@$(FILE) -F force=1 > $(HTML)

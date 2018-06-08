FILE = P1123.bs
HTML = P1123r0.html

all:
	curl -k https://api.csswg.org/bikeshed/ -F file=@$(FILE) -F force=1 > $(HTML)

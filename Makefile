install:
	bash -c "gcc src/*.c -o app `pkg-config --cflags --libs gtk+-3.0`"
	mkdir -p /usr/share/convertio
	mkdir -p /usr/share/applications
	install -Dm755 app /usr/bin/convertio
	install -Dm755 src/style.css /usr/share/convertio/style.css
	install -Dm755 src/ui.glade /usr/share/convertio/ui.glade
	install -Dm755 convertio.desktop /usr/share/applications/convertio.desktop
	rm -rf src
	rm -rf app
	rm -rf Makefile
	rm -rf convertio.desktop

uninstall:
	sudo rm -rf /usr/share/convertio
	sudo rm -rf /usr/bin/convertio
	sudo rm -rf /usr/share/applications/convertio.desktop
	rm -rf src
	rm -rf app
	rm -rf Makefile
	rm -rf convertio.desktop


clean:
	rm -f app;

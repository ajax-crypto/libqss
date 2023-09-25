# A simple Qt CSS (QSS) dialect parser

The parser is written in C++14 and parses the supported dialect of QSS i.e. Qt's CSS system. This is not exactly the 
same as CSS3 (web standards) which is a superset of QSS. As Qt Widgets library does not expose the built-in parser,
this library takes care of parsing. 

Note: This library does not actually update widgets when properties are modified. This can however be done by writing
appropriate wrappers on top of Qt widget classes.

## Building

```
mkdir build
cd build
cmake ..
make
```

## Testing

```
cd build
./test_qss
```

## Example

Parse [QDarkStyleSheet](https://github.com/ColinDuquesnoy/QDarkStyleSheet) and extract background color of QWidget:

```c++
QApplication a(argc, argv);
...
QString styleSheetLocation = QString(":qdarkstyle/dark/style.qss");
QFile styleSheetFile(styleSheetLocation);

if (!styleSheetFile.exists())
	printf("Unable to set stylesheet, file not found\n");
else
{
	QTextStream ts(&styleSheetFile);
	styleSheetFile.open(QFile::ReadOnly | QFile::Text);
	a.setStyleSheet(ts.readAll());
}
...
qss::Document document(qApp->styleSheet());
for (auto it = document.cbegin(); it < document.cend(); it++)
{
	auto fragment = it->first;
	if (fragment.selector().toString() != "QWidget") continue;
	
	auto block = fragment.block();
	for (auto it2 = block.cbegin(); it2 != block.cend(); it2++)
	{
		auto name = it2->first;
		if (name != "background-color") continue;
		
		auto value = it2->second.first;
		qDebug() << value << "\n";
		return value;
	}
}
```


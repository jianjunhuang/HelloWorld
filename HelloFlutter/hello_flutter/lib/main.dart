import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';

void main() => runApp(const MyApp());

class MyTextView extends StatelessWidget {
  const MyTextView({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    TextStyle blackStyle = const TextStyle(
        fontWeight: FontWeight.normal, fontSize: 20, color: Colors.black);
    TextStyle redStyle = const TextStyle(
        fontWeight: FontWeight.bold, fontSize: 30, color: Colors.red);
    return Column(
      children: [
        const Text(
          '文本是视图系统中的常见控件，用来显示一段特定样式的字符串，就比如Android里的TextView，或是iOS中的UILabel。',
          textAlign: TextAlign.center, //居中显示
          style: TextStyle(
              fontWeight: FontWeight.bold,
              fontSize: 20,
              color: Colors.red), //20号红色粗体展示
        ),
        Text.rich(TextSpan(children: [
          TextSpan(text: 'hi', style: blackStyle),
          TextSpan(text: 'hello', style: redStyle)
        ])),
        FloatingActionButton(onPressed: () => print("float")),
        RaisedButton(onPressed: () => print("raised")),
        ElevatedButton(
          onPressed: () => print("raised"),
          child: const Text("click"),
        ),
        TextButton(onPressed: () => print("button"), child: Text("child")),
      ],
    );
  }
}

class MyListViewTest extends StatelessWidget {
  const MyListViewTest({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Container(
      child: ListView(
        scrollDirection: Axis.horizontal,
        //item延展尺寸(宽度)
        itemExtent: 140,
        children: [
          Container(color: Colors.black),
          Container(color: Colors.red),
          Container(color: Colors.blue),
          Container(color: Colors.green),
          Container(color: Colors.yellow),
          Container(color: Colors.orange),
        ],
      ),
    );
  }
}

class MyContainerTest extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Row(
      children: [
        Expanded(
            flex: 1,
            child: Container(
                child: const Center(
                  child: Text('a long long long long long long text'),
                ),
                padding: const EdgeInsets.all(16),
                margin: const EdgeInsets.all(32),
                width: 180,
                height: 240,
                alignment: Alignment.topRight,
                decoration: BoxDecoration(
                    color: Colors.blue,
                    borderRadius: BorderRadius.circular(16)))),
        Expanded(
            flex: 2,
            child: Row(
              children: [
                Text('test 1'),
                Container(
                  color: Colors.black,
                  width: 16,
                  height: 16,
                )
              ],
            ))
      ],
    );
  }
}

class MyApp extends StatelessWidget {
  const MyApp({Key? key}) : super(key: key);

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
        title: 'Flutter Demo',
        theme: ThemeData(
          // This is the theme of your application.
          //
          // Try running your application with "flutter run". You'll see the
          // application has a blue toolbar. Then, without quitting the app, try
          // changing the primarySwatch below to Colors.green and then invoke
          // "hot reload" (press "r" in the console where you ran "flutter run",
          // or press Run > Flutter Hot Reload in a Flutter IDE). Notice that the
          // counter didn't reset back to zero; the application is not restarted.
          primarySwatch: Colors.blue,
        ),
        home: MyContainerTest());
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({Key? key, required this.title}) : super(key: key);

  // This widget is the home page of your application. It is stateful, meaning
  // that it has a State object (defined below) that contains fields that affect
  // how it looks.

  // This class is the configuration for the state. It holds the values (in this
  // case the title) provided by the parent (in this case the App widget) and
  // used by the build method of the State. Fields in a Widget subclass are
  // always marked "final".

  final String title;

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  int _counter = 0;

  void _incrementCounter() {
    setState(() {
      // This call to setState tells the Flutter framework that something has
      // changed in this State, which causes it to rerun the build method below
      // so that the display can reflect the updated values. If we changed
      // _counter without calling setState(), then the build method would not be
      // called again, and so nothing would appear to happen.
      _counter++;
    });
  }

  @override
  Widget build(BuildContext context) {
    // This method is rerun every time setState is called, for instance as done
    // by the _incrementCounter method above.
    //
    // The Flutter framework has been optimized to make rerunning build methods
    // fast, so that you can just rebuild anything that needs updating rather
    // than having to individually change instances of widgets.
    return Scaffold(
      appBar: AppBar(
        // Here we take the value from the MyHomePage object that was created by
        // the App.build method, and use it to set our appbar title.
        title: Text(widget.title),
      ),
      body: Center(
        // Center is a layout widget. It takes a single child and positions it
        // in the middle of the parent.
        child: Column(
          // Column is also a layout widget. It takes a list of children and
          // arranges them vertically. By default, it sizes itself to fit its
          // children horizontally, and tries to be as tall as its parent.
          //
          // Invoke "debug painting" (press "p" in the console, choose the
          // "Toggle Debug Paint" action from the Flutter Inspector in Android
          // Studio, or the "Toggle Debug Paint" command in Visual Studio Code)
          // to see the wireframe for each widget.
          //
          // Column has various properties to control how it sizes itself and
          // how it positions its children. Here we use mainAxisAlignment to
          // center the children vertically; the main axis here is the vertical
          // axis because Columns are vertical (the cross axis would be
          // horizontal).
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            const Text(
              'You have pushed the button this many times:',
            ),
            Text(
              '$_counter',
              style: Theme.of(context).textTheme.headline4,
            ),
          ],
        ),
      ),
      floatingActionButton: FloatingActionButton(
        onPressed: _incrementCounter,
        tooltip: 'Increment',
        child: const Icon(Icons.add),
      ), // This trailing comma makes auto-formatting nicer for build methods.
    );
  }
}

ℹ This widget does not yet exist. This is just a to-do list on how I'd like to implement the widget.

It hasn't been implemented as it is impossible to place text at arbitrary coordinates with `ss_oled`. See *App API* in todo.md.

API List widget
====

Widget headers along with their source file will be stored in /hal/ui/widget. So `ListWidget` will be under `/hal/ui/widget/list.hpp` and `/hal/ui/widget/list.cpp`.

There will also be a `ListWidgetElement` class for each list element.

See the QT list widget class for ideas: https://doc.qt.io/qt-5/qlistwidget.html

- The list widget will be a class
- The constructor will take a pointer to `app_api`, the widget's top-left x, y coordinates and width, height.
- Styling will be done by the implementation. The only exposed option would be to possibly invert the whole widget colour.
- To add elements to list (methods):
    - All return `ListWidgetElement*`
    - `element_append(ListWidgetElement*)`
    - `element_insert() // insert at specific pos`
    - `element_append_list() // add a whole array of elements (struct) to the widget`
- To scroll the widget (methods):
    - `scroll() // num of elements to scroll down by (can be negative)`
    - `scroll_to(ListWidgetElement* element) // scroll to a specific element`
- To change geometry (methods):
    - `geometry(x, y, width, height)`
- To draw (methods):
    - `paint()`
- ...

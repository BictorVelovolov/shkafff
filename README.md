В этом файле мы немного расскажем о коде для замков нашей станции

Хотим сразу сказать, что около 25% кода взято из интернета (с сайта Амперки, чей сканер мы используем, с форумов по Ардуино и из Гит хаба скачены библиотеки для работы со сканером). Оттуда взяты библиотеки, функция для сравнения ID и код для правильного запуска и настройки сканера. Все остальное написано вручную.

Данный код абсолютно работает (т.к. мы пока еще не определились с используемыми для замков сервоприводами, то в коде команды для поворота стандартных Ардуиновских сервоприводов)

На Гит Хабе лежат 2 версии кода: одна для прототипа на 2 ящика, который выставляется на стенд, а другая создана дабы подтвердить, что Ардуина способна обеспечивать даже 10 присоединенных к ней ящиков.


Планы по улучшению кода (к защите, а после нее для полноценного использования нашего проекта):


-Добавить функцию мастер ключа, которым будет карточка одного из создателей проекта, или одного из учителей, при ее использовании все шкафчики должны будут принудительно открыться (это позволит нам устранять неполадки и уносить зарядную станцию на обслуживание, не подвергая опасности чужие телефоны)



С уважением
Ученики 10х Инж и IT
Константин, Виктор и Артем.

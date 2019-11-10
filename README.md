# FaillessDB
Simple, Fast, Key-Value In-Memory Database

# Разработка клиентской части проекта

Структура клиентской части проект состоит из 3 функциональных единиц: 
- Клиент   
- Сетевой клиент
- Файловая система
 
Все единицы построены внутри собственного пространства имен, состоят структурно их пары интерфейсный класс с чистыми виртуальными методами и класс-реализация (наследуется от интерфейса).

## Клиент

Интерфейс Клиента содержит:
```c++
    class ClientInterface {
    public:
        ClientInterface() = default;
        ~ClientInterface() = default;
        virtual size_t ReadQuery( string query ) = 0;
        virtual size_t Send() = 0;
        virtual size_t Params( int argc, char **argv ) = 0;
        int Test(string query) {
            ReadQuery(query);
            return EXIT_SUCCESS;
        }
    private:
        virtual size_t ParseQuery_( string query ) = 0;
    };
```
Метод `Params` отвечает за чтение запроса пользователя из командной строки.

Метод `ParseQuery_` отвечает за разбитие считанного запроса пользователя на токены.

Метод `Send` отвечает за отправку запроса пользователя на application сервер.

Метод `ReadQuery` отвечает за чтение запроса клиента из других источни ков, отличных от команжной строки (напр. интерепретатор Lua).

Метод `Test` отвечает за пример Mock тестирования. **Должен быть удален или переписан**

Реализация интерфеса Клиента является контейнером для Сетевого клиента и Файловой системы, а также имеет актуальную конфигурацию соединения.

### TODO для интерфейса Клиента

1. Разбить методы для того, чтобы они отвечали за более атомарные задачи.
2. Переписать интерфейс клиента для возможности его тестирования.

---------

## Сетевой клиент

Интерфейс Сетевого клиента на данный момент времени отсуствует.
Реализация сетевого клиента на уровне структуры класса содержит:
```c++
class MpClient : public MpClientInterface {
public:
    explicit MpClient();

    ~MpClient();
    static void Start(const ip::tcp::endpoint& ep);

    typedef boost::system::error_code error_code;
    typedef boost::shared_ptr<MpClient> ptr;

    static ptr Start(const ip::tcp::endpoint& ep, const std::string &username);
    static void Stop();
    static bool Started();

private:
    static size_t ReadComplete_(const boost::system::error_code &err, size_t bytes);

private:
    char read_buffer_[MAX_MSG]{};
    char write_buffer_[MAX_MSG]{};
};
```

Реализация **асинхронного** Сетевого клиента повторяет опыт книги _John Torjo «Boost.Asio C++ Network Programming»_. 
Код взят непосредственно из перевода книги: https://habr.com/ru/post/195794/ (_«Boost.Asio C++ Network Programming». Глава 4: Клиент и Сервер_).

### TODO для интерфейса Сетевого клиента

1. Аргуметация в пользу использования асинхронного Сетевого клиента.
2. Переписать интерфейс клиента для возможности его тестирования.

---------

## Файловая система

Интерфейс Файловой системы содержит:
```c++
class FileSystemInterface {
public:
    FileSystemInterface() = default;
    ~FileSystemInterface() = default;

    virtual size_t ReadFile(uintptr_t data) = 0;

    virtual size_t WriteFile(uintptr_t data) = 0;
};
```

Метод `ReadFile` отвечает за чтение файла из файловой системы для последующей отправки на базу данных.

Метод `WriteFile` отвечает за запись файла, принятого из базы данных.


### TODO для интерфейса Сетевого клиента

1. Переписать интерфейс клиента для возможности его тестирования.

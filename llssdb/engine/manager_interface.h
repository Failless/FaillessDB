#ifndef FAILLESS_MANAGER_INTERFACE_H
#define FAILLESS_MANAGER_INTERFACE_H

enum command {
    SET = 0,
    GET,
    DELETE
};

class IManager : boost::noncopyable {
public:
    IManager() = default;
    virtual Execute() = 0;

};

#endif // FAILLESS_MANAGER_INTERFACE_H

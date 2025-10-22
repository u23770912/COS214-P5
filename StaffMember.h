#ifndef STAFF_MEMBER_H
#define STAFF_MEMBER_H

class CareCommand;

class StaffMember {
private:
    StaffMember* nextHandler;
    bool isBusy;

public:
    StaffMember();
    
    virtual ~StaffMember() = default;
    
    void setNext(StaffMember* next);

    bool getIsBusy() const;

    void setIsBusy(bool busy);

    
    virtual void handle(CareCommand* command);
};

#endif
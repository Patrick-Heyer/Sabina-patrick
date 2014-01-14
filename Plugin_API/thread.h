// Thread.h:
class Thread {
public:
    ..
    static void*  callMemberFunction(void *arg) {
        return ((Thread*)arg)->memberFunction();
    }

    void*         memberFunction(void);
    void          startThread(void);
};



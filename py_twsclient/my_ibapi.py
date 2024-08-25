from ibapi.client import *
from ibapi.wrapper import *
import time
import threading

def PrintTid():
    return f"ThreadID: {threading.get_native_id()}"

class IBApi(EClient, EWrapper):
    def __init__(self):
        EClient.__init__(self, self)

    def nextValidId(self, tranId):
        self.tranId = tranId
        print("NextValidId:",tranId,PrintTid())

    def nextId(self):
        self.tranId += 1
        return self.tranId

    def currentTime(self, time):
        print("Time:",time,PrintTid())

    def error(self, reqId, errorCode, errorString, advancedOrderReject):
        print(f"reqId: {reqId}, errorCode: {errorCode}, errorString: {errorString}, orderReject: {advancedOrderReject}",PrintTid())

app = IBApi()
app.connect("127.0.0.1", 7497, 0)

threading.Thread(target=app.run).start()

time.sleep(1)

for i in range(0,5):
    print(app.nextId())
    app.reqCurrentTime()

app.disconnect()
time.sleep(1)

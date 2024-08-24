from ibapi.client import *
from ibapi.wrapper import *
import time
import threading

class TestApp(EClient, EWrapper):
    def __init__(self):
        EClient.__init__(self, self)

    def nextValidId(self, orderId):
        self.orderId = orderId

    def nextId(self):
        self.orderId += 1
        return self.orderId

    def currentTime(self, time):
        print(time)

    def error(self, reqId, errorCode, errorString):
        print(f"reqId: {reqId}, errorCode: {errorCode}, errorString: {errorString}")

app = TestApp()
app.connect("127.0.0.1", 7497, 0)
threading.Thread(target=app.run).start()
time.sleep(1)

for i in range(0,5):
    print(app.nextId())
    app.reqCurrentTime()

from ibapi.client import *
from ibapi.wrapper import *
import time
import threading
from datetime import datetime

def PTid():
    timenow = datetime.now()
    formatted = timenow.strftime("%H:%M:%S.%f")[:-3]
    return f"{formatted} ThreadID: {threading.get_native_id()}"


class TCApi(EClient, EWrapper):
    def __init__(self, mgr):
        EClient.__init__(self, self)
        self.time_received = threading.Event()
        self.contract_received = threading.Event()
        self.mgr = mgr

    def nextValidId(self, tranId):
        self.tranId = tranId
        print(PTid(), "NextValidId:", tranId)

    def nextId(self):
        self.tranId += 1
        return self.tranId

    def currentTime(self, unix_time):
        formtime = datetime.fromtimestamp(unix_time)
        date_time = formtime.strftime("%Y%m%d %H:%M:%S")
        print(PTid(), "Time:", date_time,)
        self.time_received.set()

    def error(self, reqId, errorCode, errorString, advancedOrderReject):
        print(PTid(), f"reqId: {reqId}, errorCode: {errorCode}, errorString: {errorString}, orderReject: {advancedOrderReject}")

    def contractDetails(self, reqId, contractDetails):
        print(PTid(), f"Contract: {contractDetails.contract}")
        print(PTid(), f"validExchanges: {contractDetails.validExchanges}")
        print(PTid(), f"tradingHours: {contractDetails.tradingHours}")
        print(PTid(), f"liquidHours: {contractDetails.liquidHours}")

        self.mgr.addContractDetails(contractDetails.contract.conId)

    def contractDetailsEnd(self, reqId):
        print(PTid(), "End Of Contract Details")
        self.contract_received.set()

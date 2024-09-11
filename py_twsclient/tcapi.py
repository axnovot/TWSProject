from ibapi.client import *
from ibapi.wrapper import *
from ibapi.ticktype import TickTypeEnum
from tcutil import PTid, tprint
import time
import threading
from datetime import datetime

class TCApi(EClient, EWrapper):
    def __init__(self, mgr):
        EClient.__init__(self, self)
        self.information_received = threading.Event()
        self.mgr = mgr

    def nextValidId(self, tranId):
        self.tranId = tranId
        tprint("NextValidId:", tranId)

    def nextId(self):
        self.tranId += 1
        return self.tranId

    def currentTime(self, unix_time):
        formtime = datetime.fromtimestamp(unix_time)
        date_time = formtime.strftime("%Y%m%d %H:%M:%S")
        tprint("Time:", date_time,)
        self.information_received.set()

    def error(self, reqId, errorCode, errorString, advancedOrderReject):
        tprint(f"reqId: {reqId}, errorCode: {errorCode}, errorString: {errorString}, orderReject: {advancedOrderReject}")

    def contractDetails(self, reqId, contractDetails):
        tprint(f"Contract: {contractDetails.contract}")
        tprint(f"validExchanges: {contractDetails.validExchanges}")
        tprint(f"tradingHours: {contractDetails.tradingHours}")
        tprint(f"liquidHours: {contractDetails.liquidHours}")

        self.mgr.addContractDetails(contractDetails.contract.conId)

    def contractDetailsEnd(self, reqId):
        tprint("End Of Contract Details")
        self.information_received.set()

    def tickPrice(self, reqId, tickType, price, attrib):
        tprint(f"reqId: {reqId}, tickType: {TickTypeEnum.to_str(tickType)}, price: {price}, attrib: {attrib}")

    def tickSize(self, reqId, tickType, size):
        tprint(f"reqId: {reqId}, tickType: {TickTypeEnum.to_str(tickType)}, size: {size}")




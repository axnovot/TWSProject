from tcapi import IBApi
from ibapi.contract import Contract
import threading
import time

class Manager:
    def __init__(self):
        self.api = IBApi()

    def init(self):
        self.api.connect("127.0.0.1", 7497, 0)

        threading.Thread(target = self.api.run).start()
        time.sleep(1)

        self.api.reqCurrentTime()

        mycontract = Contract()
        mycontract.symbol = "IBM"
        mycontract.secType = "STK"
        mycontract.currency = "USD"
        mycontract.exchange = "SMART"

        self.api.reqContractDetails(self.api.nextId(), mycontract)

    def stop(self):
        self.api.disconnect()
        time.sleep(1)

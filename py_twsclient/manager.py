from tcapi import TCApi
from tcstock import TCStock
from ibapi.contract import Contract
import threading
import time

class Manager:
    def __init__(self):
        self.api = TCApi(self)
        self.theStock = None

    def init(self):
        self.api.connect("127.0.0.1", 7497, 0)

        threading.Thread(target = self.api.run).start()
        time.sleep(1)

        self.api.reqCurrentTime()
        self.api.time_received.wait()

        """
        mycontract = Contract()
        mycontract.symbol = "IBM"
        mycontract.secType = "STK"
        mycontract.currency = "USD"
        mycontract.exchange = "SMART"
        """
        self.theStock = TCStock()

        self.api.reqContractDetails(self.api.nextId(), self.theStock.mycontract)
        self.api.contract_received.wait()

    def addContractDetails(self, contractID):
        print(f"Manager Received Contract ID: {contractID}")
        if self.theStock:
            self.theStock.contractID = contractID
        else:
            print("TCStock Class Not Initialized")

    def stop(self):
        self.api.disconnect()
        time.sleep(1)

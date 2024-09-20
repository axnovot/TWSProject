from tcapi import TCApi
from tcstock import TCStock
from ibapi.contract import Contract
from ibapi.ticktype import TickTypeEnum
from tcutil import PTid, tprint
import threading
import time

class TCManager:
    def __init__(self):
        self.api = TCApi(self)
        self.theStock = TCStock()

    def init(self):
        self.api.connect("127.0.0.1", 7497, 0)

        threading.Thread(target = self.api.run).start()
        time.sleep(1)

        self.api.reqCurrentTime()
        self.api.information_received.wait()
        self.api.information_received.clear()


        self.api.reqContractDetails(self.api.nextId(), self.theStock.mycontract)
        self.api.information_received.wait()
        self.api.information_received.clear()


    def addContractDetails(self, contractID):
        tprint(f"Manager Received Contract ID: {contractID}")
        if self.theStock:
            self.theStock.setContractID(contractID)
        else:
            tprint("TCStock Class Not Initialized")

    def reqClosingPrices(self):
        FROZEN = 2
        DELAYED = 3

        self.api.reqMarketDataType(DELAYED)
        self.api.reqMktData(self.api.nextId(), self.theStock.contract(), "", False, False, [])

        self.api.market_data_received.wait()
        self.api.market_data_received.clear()

        tprint("Market Data Requested Successfully")

    def stop(self):
        self.api.disconnect()
        tprint("TCM Stopping")


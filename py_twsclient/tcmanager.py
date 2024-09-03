from tcapi import TCApi
from tcstock import TCStock
from ibapi.contract import Contract
from ibapi.ticktype import TickTypeEnum
from timefile import PTid
import threading
import time

class TCManager:
    def __init__(self):
        self.api = TCApi(self)
        self.theStock = None

    def init(self):
        self.api.connect("127.0.0.1", 7497, 0)

        threading.Thread(target = self.api.run).start()
        time.sleep(1)

        self.api.reqCurrentTime()
        self.api.information_received.wait()
        self.api.information_received.clear()



        self.theStock = TCStock()

        self.api.reqContractDetails(self.api.nextId(), self.theStock.mycontract)
        self.api.information_received.wait()


    def addContractDetails(self, contractID):
        print(f"Manager Received Contract ID: {contractID}")
        if self.theStock:
            self.theStock.setContractID = contractID
            """self.reqMD()"""
        else:
            print(PTid, "TCStock Class Not Initialized")

    def stop(self):
        self.api.disconnect()
        self.api.information_received.wait()

from ibapi.contract import Contract


class TCStock:
    def __init__(self):
        self.mycontract = Contract()
        self.mycontract.symbol = "AAPL"
        self.mycontract.secType = "STK"
        self.mycontract.currency = "USD"
        self.mycontract.exchange = "SMART"
        #self.mycontract.primaryExchange = "NYSE"

        self.contractID = 0

    def setContractID(self, conid):
        self.contractID = conid

    def contract(self):
        return self.mycontract

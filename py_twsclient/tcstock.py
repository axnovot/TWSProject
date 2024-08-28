from ibapi.contract import Contract


class TCStock:
    def __init__(self):
        self.mycontract = Contract()
        self.mycontract.symbol = "IBM"
        self.mycontract.secType = "STK"
        self.mycontract.currency = "USD"
        self.mycontract.exchange = "SMART"

        self.contractID = 0
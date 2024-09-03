from datetime import datetime
import threading

def PTid():
    timenow = datetime.now()
    formatted = timenow.strftime("%H:%M:%S.%f")
    return f"{formatted} ThreadID: {threading.get_native_id()}"


import yfinance as yf

def download(symbol, start="2015-01-01"):
    df = yf.download(symbol, start=start)
    df.to_csv(f"data/{symbol}.csv")
    print("Saved:", f"data/{symbol}.csv")

if __name__ == "__main__":
    download("AAPL")
    download("MSFT")
    download("^GSPC")   # S&P500 index benchmark
    download("GC=F")    # Gold futures benchmark
    download("GOOG")    # Google

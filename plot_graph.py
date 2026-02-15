import pandas as pd
import matplotlib.pyplot as plt
import sys

# Default file location
file = "../output/equity.csv"

# Allow override
if len(sys.argv) > 1:
    file = sys.argv[1]

print("Reading equity curve from:", file)

# Load equity curve
eq = pd.read_csv(file)

# Convert date column to datetime
eq["date"] = pd.to_datetime(eq["date"])

# Extract month-year
eq["month"] = eq["date"].dt.to_period("M")

# Compute average equity per month
monthly_eq = eq.groupby("month")["value"].mean()

# Plot monthly average equity
plt.figure(figsize=(12, 6))
plt.plot(monthly_eq.index.astype(str), monthly_eq.values)

plt.title("Monthly Average Portfolio Equity Curve")
plt.xlabel("Month")
plt.ylabel("Average Equity")

# Reduce x labels clutter
plt.xticks(rotation=45)

plt.grid(True)
plt.show()

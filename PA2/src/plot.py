import matplotlib.pyplot as plt
import numpy as np

# =============================================================================
# DATA INPUT SECTION - FILL IN YOUR DATA HERE
# =============================================================================

# Experiment 1 Data
exp1_customers = [1, 2, 4, 8, 16, 32, 64, 128, 256]  # Number of customers
exp1_mean_latency = [108.35, 117.28, 131.55, 152.60, 252.32, 450.83, 974.49, 1877.58, 3784.83]  # Mean latency in ms
exp1_min_latency = [84.00, 82.00, 77.00, 87.00, 93.00, 88.00, 94.00, 97.00, 96.00]    # Min latency in ms
exp1_max_latency = [2588.00, 3263.00, 4091.00, 6534.00, 71138.00, 199411.00, 64197.00, 121109.00, 133712.00]   # Max latency in ms
exp1_throughput = [9165.90, 16789.77, 30084.02, 51819.68, 62300.30, 68926.08, 64908.79, 67669.92, 64739.06]    # Throughput (requests/sec or your unit)

# Experiment 2 Data
exp2_customers = [1, 2, 4, 8, 16, 32, 64, 128, 256]
exp2_mean_latency = [312.91, 378.30, 751.95, 1514.23, 3007.48, 5876.04, 11649.05, 23341.39, 48735.47]
exp2_min_latency = [203.00, 263.00, 314.00, 319.00, 334.00, 329.00, 355.00, 423.00, 396.00]
exp2_max_latency = [3878.00, 3493.00, 6960.00, 8041.00, 13458.00, 12864.00, 432088.00, 350371.00, 580096.00]
exp2_throughput = [3187.12, 5274.28, 5312.85, 5279.30, 5317.96, 5443.78, 5444.94, 5305.96, 5246.59]

# Experiment 3 Data
exp3_customers = [1, 2, 4, 8, 16, 32, 64, 128, 256]
exp3_mean_latency = [328.49, 355.22, 366.22, 394.65, 470.17, 690.49, 1380.63, 2805.72, 5432.50]
exp3_min_latency = [221.00, 241.00, 227.00, 241.00, 241.00, 238.00, 253.00, 271.00, 258.00]
exp3_max_latency = [5952.00, 4457.00, 7641.00, 12721.00, 11430.00, 11794.00, 72653.00, 28226.00, 415678.00]
exp3_throughput = [3037.23, 5559.13, 10858.43, 19982.09, 33126.51, 44968.52, 45594.35, 45410.45, 46644.74]

# Experiment 4 Data
exp4_customers = [1, 2, 4, 8, 16, 32, 64, 128, 256]
exp4_mean_latency = [322.69, 328.62, 358.72, 393.63, 480.33, 651.80, 1580.01, 2563.17, 6052.92]
exp4_min_latency = [225.00, 221.00, 239.00, 244.00, 239.00, 236.00, 252.00, 225.00, 239.00]
exp4_max_latency = [5766.00, 3455.00, 6122.00, 6261.00, 13484.00, 50130.00, 301136.00, 626004.00, 548104.00]
exp4_throughput = [3091.35, 6000.98, 10994.95, 20197.04, 32839.66, 47575.64, 39610.18, 41737.08, 41041.98]

# =============================================================================
# PLOTTING FUNCTIONS
# =============================================================================

def plot_latency(customers, mean_lat, min_lat, max_lat, exp_num):
    """Plot latency graph with mean, min, and max values"""
    plt.figure(figsize=(10, 6))
    
    # Create x-axis positions with equal spacing
    x_pos = np.arange(len(customers))
    
    plt.plot(x_pos, mean_lat, marker='o', linewidth=2, markersize=8, label='Mean Latency')
    plt.plot(x_pos, min_lat, marker='s', linewidth=2, markersize=8, label='Min Latency')
    plt.plot(x_pos, max_lat, marker='^', linewidth=2, markersize=8, label='Max Latency')
    
    # Set logarithmic y-axis (base 10)
    plt.yscale('log', base=10)

    plt.xlabel('Number of Customers', fontsize=12)
    plt.ylabel('Latency (microseconds)', fontsize=12)
    plt.title(f'Experiment {exp_num}: Latency vs Number of Customers', fontsize=14, fontweight='bold')
    plt.xticks(x_pos, customers)  # Set custom x-tick labels
    plt.legend(fontsize=10)
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    
    plt.savefig(f'experiment_{exp_num}_latency.png', dpi=300, bbox_inches='tight')
    plt.show()

def plot_throughput(customers, throughput, exp_num):
    """Plot throughput graph"""
    plt.figure(figsize=(10, 6))
    
    # Create x-axis positions with equal spacing
    x_pos = np.arange(len(customers))
    
    plt.plot(x_pos, throughput, marker='o', linewidth=2, markersize=8, color='green')
    
    plt.xlabel('Number of Customers', fontsize=12)
    plt.ylabel('Throughput (requests/sec)', fontsize=12)
    plt.title(f'Experiment {exp_num}: Throughput vs Number of Customers', fontsize=14, fontweight='bold')
    plt.xticks(x_pos, customers)  # Set custom x-tick labels
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    
    plt.savefig(f'experiment_{exp_num}_throughput.png', dpi=300, bbox_inches='tight')
    plt.show()

# =============================================================================
# GENERATE ALL PLOTS
# =============================================================================

print("Generating plots for all experiments...")

# Experiment 1
print("\nGenerating Experiment 1 graphs...")
plot_latency(exp1_customers, exp1_mean_latency, exp1_min_latency, exp1_max_latency, 1)
plot_throughput(exp1_customers, exp1_throughput, 1)

# Experiment 2
print("\nGenerating Experiment 2 graphs...")
plot_latency(exp2_customers, exp2_mean_latency, exp2_min_latency, exp2_max_latency, 2)
plot_throughput(exp2_customers, exp2_throughput, 2)

# Experiment 3
print("\nGenerating Experiment 3 graphs...")
plot_latency(exp3_customers, exp3_mean_latency, exp3_min_latency, exp3_max_latency, 3)
plot_throughput(exp3_customers, exp3_throughput, 3)

# Experiment 4
print("\nGenerating Experiment 4 graphs...")
plot_latency(exp4_customers, exp4_mean_latency, exp4_min_latency, exp4_max_latency, 4)
plot_throughput(exp4_customers, exp4_throughput, 4)

print("\nAll plots generated successfully!")
print("Graphs saved as PNG files in the current directory.")
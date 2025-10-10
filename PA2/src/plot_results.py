#!/usr/bin/env python3
"""
Plot performance results from robot factory experiments
"""

import matplotlib.pyplot as plt
import pandas as pd
import sys
import os

def load_experiment_data(filename):
    """Load experiment data from TSV file"""
    try:
        df = pd.read_csv(filename, sep='\t')
        return df
    except Exception as e:
        print(f"Error loading {filename}: {e}")
        return None

def plot_latency(experiments, output_file):
    """Plot latency graph (mean, min, max)"""
    plt.figure(figsize=(12, 6))
    
    colors = ['blue', 'red', 'green', 'purple']
    markers = ['o', 's', '^', 'd']
    
    for i, (name, df) in enumerate(experiments.items()):
        if df is not None:
            plt.plot(df['customers'], df['avg_latency'], 
                    label=f'{name} (Avg)', 
                    marker=markers[i], color=colors[i], linewidth=2)
            plt.plot(df['customers'], df['min_latency'], 
                    label=f'{name} (Min)', 
                    marker=markers[i], color=colors[i], 
                    linewidth=1, linestyle='--', alpha=0.5)
            plt.plot(df['customers'], df['max_latency'], 
                    label=f'{name} (Max)', 
                    marker=markers[i], color=colors[i], 
                    linewidth=1, linestyle=':', alpha=0.5)
    
    plt.xlabel('Number of Customers', fontsize=12)
    plt.ylabel('Latency (μs)', fontsize=12)
    plt.title('Order Latency vs Number of Customers', fontsize=14, fontweight='bold')
    plt.legend(loc='best', fontsize=9)
    plt.grid(True, alpha=0.3)
    plt.xscale('log', base=2)
    plt.yscale('log')
    plt.tight_layout()
    plt.savefig(output_file, dpi=300)
    print(f"Saved: {output_file}")
    plt.close()

def plot_throughput(experiments, output_file):
    """Plot throughput graph"""
    plt.figure(figsize=(12, 6))
    
    colors = ['blue', 'red', 'green', 'purple']
    markers = ['o', 's', '^', 'd']
    
    for i, (name, df) in enumerate(experiments.items()):
        if df is not None:
            plt.plot(df['customers'], df['throughput'], 
                    label=name, 
                    marker=markers[i], color=colors[i], 
                    linewidth=2, markersize=8)
    
    plt.xlabel('Number of Customers', fontsize=12)
    plt.ylabel('Throughput (orders/sec)', fontsize=12)
    plt.title('Factory Throughput vs Number of Customers', fontsize=14, fontweight='bold')
    plt.legend(loc='best', fontsize=10)
    plt.grid(True, alpha=0.3)
    plt.xscale('log', base=2)
    plt.tight_layout()
    plt.savefig(output_file, dpi=300)
    print(f"Saved: {output_file}")
    plt.close()

def plot_individual_experiment(df, exp_name, output_dir):
    """Plot individual experiment with both latency and throughput"""
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 5))
    
    # Latency plot
    ax1.plot(df['customers'], df['avg_latency'], 
            label='Average', marker='o', linewidth=2, color='blue')
    ax1.plot(df['customers'], df['min_latency'], 
            label='Minimum', marker='s', linewidth=1, linestyle='--', color='green')
    ax1.plot(df['customers'], df['max_latency'], 
            label='Maximum', marker='^', linewidth=1, linestyle=':', color='red')
    ax1.set_xlabel('Number of Customers', fontsize=11)
    ax1.set_ylabel('Latency (μs)', fontsize=11)
    ax1.set_title(f'{exp_name} - Latency', fontsize=12, fontweight='bold')
    ax1.legend()
    ax1.grid(True, alpha=0.3)
    ax1.set_xscale('log', base=2)
    ax1.set_yscale('log')
    
    # Throughput plot
    ax2.plot(df['customers'], df['throughput'], 
            marker='o', linewidth=2, color='purple', markersize=8)
    ax2.set_xlabel('Number of Customers', fontsize=11)
    ax2.set_ylabel('Throughput (orders/sec)', fontsize=11)
    ax2.set_title(f'{exp_name} - Throughput', fontsize=12, fontweight='bold')
    ax2.grid(True, alpha=0.3)
    ax2.set_xscale('log', base=2)
    
    plt.tight_layout()
    output_file = os.path.join(output_dir, f'{exp_name.lower().replace(" ", "_")}.png')
    plt.savefig(output_file, dpi=300)
    print(f"Saved: {output_file}")
    plt.close()

def main():
    # Define data directory
    data_dir = "experiment_results"
    
    if not os.path.exists(data_dir):
        print(f"Error: Directory '{data_dir}' not found")
        print("Please run the experiments first using run_experiments.sh")
        sys.exit(1)
    
    # Load all experiment data
    experiments = {
        'Exp1: Regular': load_experiment_data(f'{data_dir}/experiment1_data.tsv'),
        'Exp2: Special (1 Expert)': load_experiment_data(f'{data_dir}/experiment2_data.tsv'),
        'Exp3: Special (16 Experts)': load_experiment_data(f'{data_dir}/experiment3_data.tsv'),
        'Exp4: Special (Matching)': load_experiment_data(f'{data_dir}/experiment4_data.tsv')
    }
    
    # Check if any data was loaded
    if all(df is None for df in experiments.values()):
        print("Error: Could not load any experiment data")
        sys.exit(1)
    
    # Create plots directory
    plots_dir = f'{data_dir}/plots'
    os.makedirs(plots_dir, exist_ok=True)
    
    print("\nGenerating plots...")
    print("=" * 50)
    
    # Plot combined latency comparison
    plot_latency(experiments, f'{plots_dir}/all_experiments_latency.png')
    
    # Plot combined throughput comparison
    plot_throughput(experiments, f'{plots_dir}/all_experiments_throughput.png')
    
    # Plot individual experiments
    for name, df in experiments.items():
        if df is not None:
            plot_individual_experiment(df, name, plots_dir)
    
    print("=" * 50)
    print(f"\nAll plots saved to: {plots_dir}/")
    print("\nGenerated files:")
    print("  - all_experiments_latency.png (comparison)")
    print("  - all_experiments_throughput.png (comparison)")
    print("  - exp1_regular.png")
    print("  - exp2_special_(1_expert).png")
    print("  - exp3_special_(16_experts).png")
    print("  - exp4_special_(matching).png")
    
    # Print summary statistics
    print("\n" + "=" * 50)
    print("SUMMARY STATISTICS")
    print("=" * 50)
    
    for name, df in experiments.items():
        if df is not None:
            print(f"\n{name}:")
            print(f"  Max Throughput: {df['throughput'].max():.2f} orders/sec "
                  f"(at {df.loc[df['throughput'].idxmax(), 'customers']} customers)")
            print(f"  Min Avg Latency: {df['avg_latency'].min():.2f} μs "
                  f"(at {df.loc[df['avg_latency'].idxmin(), 'customers']} customers)")
            print(f"  Max Avg Latency: {df['avg_latency'].max():.2f} μs "
                  f"(at {df.loc[df['avg_latency'].idxmax(), 'customers']} customers)")

if __name__ == '__main__':
    main()

# Run: python3 plot_results.py
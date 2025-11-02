#!/usr/bin/env python3
"""
Write Performance Test - Multi-Factory Robot System
Measures latency and throughput for robot orders with varying number of servers
"""

import matplotlib.pyplot as plt
import numpy as np


def plot_write_performance(num_servers, latencies, throughputs, save_file=None):
    """Plot write performance graphs"""
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 5))
    
    # Graph 1: Latency vs Number of Servers
    ax1.plot(num_servers, latencies, 'o-', linewidth=2, markersize=8, 
             color='#2E86AB', label='Average Latency')
    ax1.set_xlabel('Number of Servers', fontsize=12, fontweight='bold')
    ax1.set_ylabel('Average Latency (μs)', fontsize=12, fontweight='bold')
    ax1.set_title('Write Latency vs Number of Servers\n(8 customers, robot orders)', 
                  fontsize=13, fontweight='bold')
    ax1.grid(True, alpha=0.3, linestyle='--')
    ax1.set_xticks(num_servers)
    ax1.legend(loc='upper left', fontsize=10)
    
    # Add value labels on points
    for i, (x, y) in enumerate(zip(num_servers, latencies)):
        ax1.annotate(f'{y:.1f}', xy=(x, y), xytext=(0, 10), 
                    textcoords='offset points', ha='center', fontsize=10)
    
    # Graph 2: Throughput vs Number of Servers
    ax2.plot(num_servers, throughputs, 's-', linewidth=2, markersize=8, 
             color='#A23B72', label='Throughput')
    ax2.set_xlabel('Number of Servers', fontsize=12, fontweight='bold')
    ax2.set_ylabel('Throughput (ops/sec)', fontsize=12, fontweight='bold')
    ax2.set_title('Write Throughput vs Number of Servers\n(8 customers, robot orders)', 
                  fontsize=13, fontweight='bold')
    ax2.grid(True, alpha=0.3, linestyle='--')
    ax2.set_xticks(num_servers)
    ax2.legend(loc='upper right', fontsize=10)
    
    # Add value labels on points
    for i, (x, y) in enumerate(zip(num_servers, throughputs)):
        ax2.annotate(f'{y:.1f}', xy=(x, y), xytext=(0, -15), 
                    textcoords='offset points', ha='center', fontsize=10)
    
    plt.tight_layout()
    
    if save_file:
        plt.savefig(save_file, dpi=300, bbox_inches='tight')
        print(f"\nGraph saved to: {save_file}")
    
    plt.show()


def main():
    print("\n" + "="*60)
    print("WRITE PERFORMANCE MEASUREMENT SCRIPT")
    print("="*60)
    
    num_servers = [1, 2, 3]
    latencies = [133.545, 341.729, 831.759]
    throughputs = [56956.550, 23350.995, 9582.318]
    
    print("\n" + "="*60)
    print("DATA SUMMARY")
    print("="*60)
    print(f"{'Servers':<10} {'Latency (μs)':<20} {'Throughput (ops/s)':<20}")
    print("-" * 60)
    for n, lat, thr in zip(num_servers, latencies, throughputs):
        print(f"{n:<10} {lat:<20.3f} {thr:<20.3f}")
    print("="*60)
    
    save = input("\nSave graph to file? (y/n): ").lower().strip()
    save_file = "write_performance.png" if save == 'y' else None
    
    plot_write_performance(num_servers, latencies, throughputs, save_file)
    
    print("\nAnalysis:")
    print(f"- Latency increase from 1 to 3 servers: {((latencies[2]-latencies[0])/latencies[0]*100):.1f}%")
    print(f"- Throughput decrease from 1 to 3 servers: {((throughputs[0]-throughputs[2])/throughputs[0]*100):.1f}%")
    print("\nExpected Behavior:")
    print("- Latency should increase with more servers (sequential replication overhead)")
    print("- Throughput should decrease as latency increases")


if __name__ == "__main__":
    main()
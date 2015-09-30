### Libraries
# Standard libraries
import numpy as np

# Third-party libraries
import skfuzzy as fuzz
import matplotlib.pyplot as plt

def visualize_mf(b, inputs):

    fig, (ax0, ax1, ax2) = plt.subplots(nrows=3, figsize=(8, 5))
    ax0.plot(inputs[0], b[0][0], 'g', linewidth=1.5, label= '-ve Medium')
    ax0.plot(inputs[0], b[0][1], 'r', linewidth=1.5, label= '-ve small')
    ax0.plot(inputs[0], b[0][2], 'c', linewidth=1.5, label= 'zero')
    ax0.plot(inputs[0], b[0][3], 'm', linewidth=1.5, label= '+ve small')
    ax0.plot(inputs[0], b[0][4], 'y', linewidth=1.5, label= '+ve Medium')
    ax0.set_title('Error')
    # ax0.legend()

    ax1.plot(inputs[1], b[1][0], 'g', linewidth=1.5, label= '-ve Medium')
    ax1.plot(inputs[1], b[1][1], 'r', linewidth=1.5, label= '-ve small')
    ax1.plot(inputs[1], b[1][2], 'c', linewidth=1.5, label= 'zero')
    ax1.plot(inputs[1], b[1][3], 'm', linewidth=1.5, label= '+ve small')
    ax1.plot(inputs[1], b[1][4], 'y', linewidth=1.5, label= '+ve Medium')
    ax1.set_title('Del_Error')
    # ax1.legend()
  
    ax2.plot(inputs[2], b[2][0], 'g', linewidth=1.5, label= '-ve Medium')
    ax2.plot(inputs[2], b[2][1], 'r', linewidth=1.5, label= '-ve small')
    ax2.plot(inputs[2], b[2][2], 'c', linewidth=1.5, label= 'zero')
    ax2.plot(inputs[2], b[2][3], 'm', linewidth=1.5, label= '+ve small')
    ax2.plot(inputs[2], b[2][4], 'y', linewidth=1.5, label= '+ve Medium')
    ax2.set_title('Output')
    # ax2.legend()

    # Turn off top/right axes
    for ax in (ax0, ax1, ax2):
        ax.spines['top'].set_visible(False)
        ax.spines['right'].set_visible(False)
        ax.get_xaxis().tick_bottom()
        ax.get_yaxis().tick_left()
    plt.tight_layout()

def visualize_output(b, inputs, output, out_final, aggregated):
    # Visualize this
    out_activation = fuzz.interp_membership(inputs[2], aggregated, out_final) # for plot
    fig, (ax3, ax4) = plt.subplots(nrows=2, figsize=(8, 5))
    output0 = np.zeros_like(inputs[2])
    ax3.fill_between(inputs[2], output0, output[0], facecolor='g', alpha=0.7)
    ax3.plot(inputs[2], b[2][0], 'g', linewidth=0.5, linestyle='--' )
    ax3.fill_between(inputs[2], output0, output[1], facecolor='r', alpha=0.7)
    ax3.plot(inputs[2], b[2][1], 'r', linewidth=0.5, linestyle='--')
    ax3.fill_between(inputs[2], output0, output[2], facecolor='k', alpha=0.7)
    ax3.plot(inputs[2], b[2][2], 'c', linewidth=0.5, linestyle='--' )
    ax3.fill_between(inputs[2], output0, output[3], facecolor='m', alpha=0.7)
    ax3.plot(inputs[2], b[2][3], 'm', linewidth=0.5, linestyle='--')
    ax3.fill_between(inputs[2], output0, output[4], facecolor='c', alpha=0.7)
    ax3.plot(inputs[2], b[2][4], 'y', linewidth=0.5, linestyle='--')
    ax3.set_title('Output membership activity')
   
    ax4.plot(inputs[2], b[2][0], 'g', linewidth=0.5, linestyle='--', )
    ax4.plot(inputs[2], b[2][1], 'r', linewidth=0.5, linestyle='--', )
    ax4.plot(inputs[2], b[2][2], 'c', linewidth=0.5, linestyle='--', )
    ax4.plot(inputs[2], b[2][3], 'm', linewidth=0.5, linestyle='--', )
    ax4.plot(inputs[2], b[2][4], 'y', linewidth=0.5, linestyle='--', )
    ax4.fill_between(inputs[2], output0, aggregated, facecolor='g', alpha=0.7)
    ax4.plot([out_final, out_final], [0, out_activation], 'k', linewidth=1.5, alpha=0.9)
    ax4.set_title('Aggregated membership and result (line)')
    for ax in (ax3, ax4):
        ax.spines['top'].set_visible(False)
        ax.spines['right'].set_visible(False)
        ax.get_xaxis().tick_bottom()
        ax.get_yaxis().tick_left()

    plt.tight_layout()
    plt.show()
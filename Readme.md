# Audio-Crypt Data Encoder

The aim of this program is to be able to encode data within an arbitrary audio signal while minimally disrupting the overall sound of the audio.  This is achieved through introducing several reserved channels through which signals are modulated to transmit data.

## Encoder

The encoder takes in a sound buffer from or a raw sound file and zeroes out one or more frequency bands.  Data is then sent across these bands through regular pulses.  Data is also sent redundantly across multiple bands to minimize data loss due to noise.


## Decoder

The decoder takes in a different sound buffer from the microphone and attempts to decode any data that was encoded.  First, the raw sound data is passed through a *fast fourier transform* to isolate its different frequencies at each time slice.  Next, the amplitudes of the reserved frequencies are measured at regular intervals.  By keeping track of the high and low ranges of these frequencies, binary data can be recovered, minus data that is lost to noise.


> [!NOTE]
> This project is currently in-progress and is not yet fully functional.  This banner will be removed as the project moves closer to completion.
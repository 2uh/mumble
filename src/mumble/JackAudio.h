/* Copyright (C) 2011, Benjamin Jemlich <pcgod@users.sourceforge.net>
   Copyright (C) 2011, Filipe Coelho <falktx@gmail.com>
   Copyright (C) 2015, Mikkel Krautz <mikkel@krautz.dk>
   Copyright (C) 2018, Bernd Buschinski <b.buschinski@gmail.com>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef MUMBLE_MUMBLE_JACKAUDIO_H_
#define MUMBLE_MUMBLE_JACKAUDIO_H_

#include "AudioInput.h"
#include "AudioOutput.h"

#include <jack/jack.h>

#define JACK_MAX_OUTPUT_PORTS 2

class JackAudioOutput;
class JackAudioInput;

class JackAudioSystem : public QObject {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(JackAudioSystem)
	protected:
		bool bActive;
		jack_client_t* client;
		jack_port_t* in_port;
		jack_port_t* out_ports[JACK_MAX_OUTPUT_PORTS];
		jack_default_audio_sample_t* output_buffer;
		unsigned int iOutPorts;
		jack_nframes_t iBufferSize;

		static int process_callback(jack_nframes_t nframes, void *arg);
		static int srate_callback(jack_nframes_t frames, void *arg);
		static int buffer_size_callback(jack_nframes_t frames, void *arg);
		static void shutdown_callback(void *arg);
	public:
		QHash<QString, QString> qhInput;
		QHash<QString, QString> qhOutput;
		bool bJackIsGood;
		int iSampleRate;
		QMutex qmWait;
		QWaitCondition qwcWait;

		void init_jack();
		void close_jack();

		void auto_connect_ports();

		void activate();

		void allocOutputBuffer(jack_nframes_t frames);

		void setNumberOfOutPorts(unsigned int ports);
		unsigned int numberOfOutPorts() const;

		JackAudioSystem();
		~JackAudioSystem();
};

class JackAudioInput : public AudioInput {
	friend class JackAudioSystem;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(JackAudioInput)
	protected:
		QMutex qmMutex;
		QWaitCondition qwcWait;
	public:
		JackAudioInput();
		~JackAudioInput() Q_DECL_OVERRIDE;
		void run() Q_DECL_OVERRIDE;
};

class JackAudioOutput : public AudioOutput {
	friend class JackAudioSystem;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(JackAudioOutput)
	protected:
		QMutex qmMutex;
		QWaitCondition qwcWait;
	public:
		JackAudioOutput();
		~JackAudioOutput() Q_DECL_OVERRIDE;
		void run() Q_DECL_OVERRIDE;
};

#endif

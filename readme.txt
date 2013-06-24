mpg123 0.59m for OS/2  (mpg123/2 0.20)
Modified and ported by Samuel Audet <guardia@cam.org> (C) 1998

Introduction
============

I got sooo fed up with other ports (and their authors) of other MP3 players
for OS/2 that I just decided to kludge mine up.  Here are the reasons for
those interested:

- Nobody could (nor wanted to) get a working dynamic priority engine.  It WAS
  quite annoying having to chose between a slowly responsive system and
  skipping MP3s.  but no more!  mpg123/2 is here!

- Not a freaking single player could tell me the remaining time of an MP3,
  come on guys!  Even the cheapest CD player can do that.

- There were no players that could fast forward, rewind, or jump to an
  arbitrary value in an MP3 file, but it is now a reality!  Now we just
  need get something like XAudio or X11Amp to easily get all that juicy
  stuff without using the current kludges in mpg123.  Hey BTW, XAudio dudes
  are looking for a FREE copy of Warp 3 or 4 (at least Gilles Boccon-Gibod
  <bok@bok.net> is).  Check their web page http://www.xaudio.com .  If you
  send them something, let me know, I'll guide them to program in OS/2.

- No players could smooothly play an MP3 file... I couldn't even do a "dir"
  without it jerking.  But now, you can even play DOS games with the
  remaining CPU time! (although some games' sound engine don't like it
  anyway).

- And last but not least, mpg123/2 opens the audio device in shareable mode
  by default.  What's the use of having a brain and human ear that can
  differentiate different sounds in the a stream if you have your sound
  device locked to play ONE damn MP3!?!?  It doesn't make sense to me. And
  you can also choose which sound card to use.

The EXE has been compiled with Pentium optimized GCC 1.0, and linked with
ILINK 01.08.r1a_CTC308c (?), but to save space, as usual you will need the

                           ************
                           *-=LATEST=-* (ie.: the most recent version)
                           ************
EMX runtime libraries.
ftp://hobbes.nmsu.edu/pub/os2/dev/emx/v0.9c/emxrt.zip
GCC290.DLL is a new DLL like the EMX ones, so you might as well keep it.

I'm looking for some information on how to program an equalizer (the one in
mpg123 source code is a sound mangler).


Notes on features
=================

Read mpg123.txt for things not discussed here.

Dynamic priority boost
----------------------

What it does, is that it checks if the second ahead buffer is filled, if it
is not, it boosts the decoder priority.  When the decoder realizes it has
the third ahead buffer filled, it drops to normal priority. The second
buffer ahead is important here.  During a boost, priority should normally
be in time critical class, where the decoder can get all the CPU attention
it needs.  Up there, if the CPU is fast enough to decode and fill the
second ahead buffer before the first ahead buffer is played (which is
always the case if your CPU can play MP3s in realtime in the first place),
you can easily imagine it's impossible for mpg123/2 to skip in those
conditions.  I have played *2* MP3s on my P150 and watched a "dir /s"
smooothly and happily scrolling in the foreground without either MP3
skipping... I think that's pretty convincing.

I know hardware level bottlenecks which could make mpg123/2 skip, but they
are the same ones that can make a Zmodem transfer give "CRC32 errors", and
a PPP connection complains about "Invalid FCS":

- You have a polling device (like for example old old 2x CD-ROMs, and
  PRINT01.SYS without /IRQ).
- You have a video card that supports "automatic PCI bus retry", but your
  motherboard stops processing during those retries.  Disable that feature.
- You have an ATI Barf64 and you are experiencing a bug in the video driver
  using software mouse pointers (ie.: colored mouse pointers).
- Your motherboard or controller card plain sucks.

I recommend the default values that are class Time Critical delta 0 for
boost and Regular delta 31 for normal.  The former leaves higher priority
tasks like multimedia do their job.  The latter permits mpg123/2 to have
priority over hogging DOS and Win-OS/2 sessions while leaving foreground
OS/2 application responsive.

However, I recently found out that dynamically resizing a heavy window
(like a webbrowser or wordprocessor) hogs the CPU beyong belief.  Rest
calm, this is not a failure of the Dynamic Priority Boost.  You can try it
with mpg123 running with -b 5 -B 330 -N 330 and it will still skip.

The format for the priority parameters -B and -N is [class number][delta number]

class number in order of priority: 1,2,4,3
delta number varies from -31 to 31

For example:  use normal priority Foreground delta -31

[C:\]mpg123 -N 4-31

If you don't want to use dynamic priority boost, just set -B and -N to the
same value.


Buffers
-------

You can specify the number of audio buffer to fill in advance.  The default
is 32, and since each buffer is 16KB, this gives about 3 seconds decoder
independancy to the audio engine before a priority boost or a skip occurs
with 44.1kHz, 16bit and stereo output.

The recommended minimum is 5, and maximum is 200.


Rewind, Fast Forward, Position Jump
-----------------------------------

These are only "kludges".  They kind of work fine on consistent MPEG stream,
but MPEG streams are not like straight PCM files.  They contain packets all
over the place.  To make it work properly, I'd have to read all of them,
and it would make the player seek a lot slower.

The Jump to accepts seconds, or minutes and seconds separated by a colon.

ie.: Jump to: 1:50  or Jump to: 110


The Played, Remaining and Total timers
--------------------------------------

None of them relies on the system timer.  It only counts the number of
frames that have passed by (that is, played by the audio device, so in a
certain sense, the audio device determines where we are).  The calculation
seems to work fine on the MPEG streams I tried, but because of the above,
the timers might get lost on inconsistent MPEG streams.

It should also be noted that the Total time is updated each frame, so if
you are currently playing an MP3 being d/led, the total time will increase
with it.

Of course, they make mpg123 use a tiny bit more CPU time...


MP3 filenames and playlists
---------------------------

Ok, a nice little feature requested by William Young <young@phonet.com>.
You don't have to specify ".mp3", mpg123/2 will automatically append it if
it doesn't find the file.  You can also use wildcards.

A trick to easily generate playlists:

[C:\MP3]dir /f/-p > playlist

Use /s for all subdirs.

MP3->WAV decoder
----------------

Yes yes, you can use MPG123 as a fast MP3->WAV decoder now!  Just use the
-w parameter and specify a directory to place the WAV files.  These WAV
files are in Microsoft PCM format, since it seems everything works with
that.


Well have phun!  Check out pm123 which is using this engine, but with a
much nicer GUI frontend!  http://www.teamos2.sci.fi/pm123


Legal stuff
===========

This freeware product is used at your own risk, although it is highly
improbable it can cause any damage.

If you plan on copying me, please give me the credits, thanks.

Oh and SDG you are not allowed to use this sound engine until you implement
all the features in here AND LET ME TRY IT BEFORE YOU DISTRIBUTE IT. That
should do it. :)


Contacting the author
=====================

Samuel Audet

E-mail:      guardia@cam.org
Homepage:    http://www.cam.org/~guardia
IRC nick:    Guardian_ (be sure it's me before starting asking questions though)
Talk Client: Guardian@guardian.dyn.ml.org

Snail Mail:

   377, rue D'Argenteuil
   Laval, Quebec
   H7N 1P7   CANADA


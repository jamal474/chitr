import { useState } from 'react';
import { Play, ArrowUpRight } from 'lucide-react';
import Shell from './Shell';
import Reveal from './Reveal';
import SectionMarker from './SectionMarker';
import { YOUTUBE_ID, YOUTUBE_URL } from '@/data/site';
import { asset } from '@/lib/utils';

/** Thumbnail facade that swaps in the YouTube iframe only once clicked. */
export default function Demo() {
  const [playing, setPlaying] = useState(false);

  return (
    <Shell as="section" id="demo" className="pt-24 md:pt-32">
      <SectionMarker
        number="02"
        eyebrow="Demo"
        title="See it run"
        note="Three minutes: opening files, stepping across a folder, and driving playback from the keyboard."
      />

      <Reveal>
        <div className="frame aspect-video relative group">
          {playing ? (
            <iframe
              className="absolute inset-0 w-full h-full"
              src={`https://www.youtube-nocookie.com/embed/${YOUTUBE_ID}?autoplay=1&rel=0`}
              title="Chitr demo"
              allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
              allowFullScreen
            />
          ) : (
            <button
              type="button"
              onClick={() => setPlaying(true)}
              className="absolute inset-0 w-full h-full"
              aria-label="Play the Chitr demo video"
            >
              <img
                src={asset('demo-thumbnail.png')}
                alt=""
                className="absolute inset-0 w-full h-full object-cover transition-transform duration-[900ms] ease-out group-hover:scale-[1.03]"
              />
              <span className="absolute inset-0 bg-ink/25 transition-colors duration-500 group-hover:bg-ink/35" />
              <span className="absolute inset-0 grid place-items-center">
                <span className="relative grid place-items-center size-20 rounded-full bg-accent text-paper-soft transition-transform duration-500 group-hover:scale-110">
                  <span className="play-ring animate-pulse-ring" />
                  <Play size={26} strokeWidth={1.5} fill="currentColor" className="translate-x-[2px]" />
                </span>
              </span>
            </button>
          )}
        </div>
      </Reveal>

      <Reveal delay={100} className="mt-4 flex flex-wrap items-baseline justify-between gap-3">
        <a
          href={YOUTUBE_URL}
          target="_blank"
          rel="noreferrer"
          className="fx-navlink fx-eyebrow text-ink inline-flex items-baseline gap-1"
        >
          Watch on YouTube <ArrowUpRight size={12} strokeWidth={1.6} />
        </a>
      </Reveal>
    </Shell>
  );
}

import { useMemo } from 'react';
import { ArrowDown, Play } from 'lucide-react';
import Shell from './Shell';
import Reveal from './Reveal';
import TiltFrame from './TiltFrame';
import { detectPlatform, type ReleaseState } from '@/lib/useRelease';
import { LATEST_URL, REPO_URL, shot } from '@/data/site';

interface HeroProps {
  release: ReleaseState;
}

export default function Hero({ release }: HeroProps) {
  const platform = useMemo(detectPlatform, []);
  const primary = platform ? release.downloads[platform] : null;

  const facts: [string, string][] = [
    ['Version', release.version],
    ['Language', 'C++ 17'],
    ['Toolkit', 'wxWidgets 3.2'],
    ['Platforms', 'macOS · Windows · Linux'],
    ['License', 'GPL-3.0'],
  ];

  return (
    <Shell as="section" id="top" className="pt-10 md:pt-14 pb-10">
      <div className="grid grid-cols-1 md:grid-cols-[1.55fr_1fr] gap-10 md:gap-16 items-start">
        <Reveal>
          <p className="fx-eyebrow text-accent mb-6">§ 00 — Media player</p>

          <h1 className="fx-display text-display-lg text-ink leading-[0.92]">
            A media player,
            <br />
            <span className="fx-display-i text-accent">written in C++.</span>
          </h1>

          <p className="fx-body text-[1.0625rem] text-ink-soft mt-7 max-w-reading pretty">
            Chitr plays images, video and audio in a native window on macOS, Windows and Linux.
            It uses the media framework each system already ships, so there is{' '}
            <span className="fx-mark">no browser engine</span> and no runtime to install beside it.
          </p>

          <div className="mt-9 flex flex-wrap items-center gap-3">
            <a href={primary?.url ?? LATEST_URL} className="btn fx-eyebrow">
              {primary ? `Download for ${primary.label}` : 'Download'}
              <ArrowDown size={14} strokeWidth={2} className="btn-arrow-down" />
            </a>
            <a href="#demo" className="btn-ghost fx-eyebrow">
              <Play size={13} strokeWidth={2} fill="currentColor" />
              Watch the demo
            </a>
          </div>

          <p className="fx-meta text-ink-dim mt-5">
            Free and open source ·{' '}
            <a href={REPO_URL} target="_blank" rel="noreferrer" className="fx-navlink text-ink-muted">
              source on GitHub
            </a>
          </p>
        </Reveal>

        <Reveal delay={140} as="aside" className="md:pt-2">
          <p className="fx-eyebrow text-ink-dim mb-4">At a glance</p>
          <dl className="border-t border-line">
            {facts.map(([key, value]) => (
              <div
                key={key}
                className="grid grid-cols-[6.5rem_1fr] gap-3 py-2.5 border-b border-line-soft"
              >
                <dt className="fx-meta text-ink-dim">{key}</dt>
                <dd className="fx-body text-[0.95rem] text-ink">{value}</dd>
              </div>
            ))}
          </dl>
        </Reveal>
      </div>

      <Reveal delay={220} className="mt-12 md:mt-16 max-w-[920px] mx-auto">
        <TiltFrame>
          <div className="frame frame-lift">
            <img
              src={shot('hero')}
              alt="The Chitr window playing a video, with the seek bar and timer visible"
              className="block w-full h-auto"
            />
          </div>
        </TiltFrame>
      </Reveal>
    </Shell>
  );
}

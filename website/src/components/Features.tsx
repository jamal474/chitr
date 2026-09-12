import Shell from './Shell';
import Reveal from './Reveal';
import SectionMarker from './SectionMarker';
import SwapFrame from './SwapFrame';
import { cn } from '@/lib/utils';
import { featureGroups } from '@/data/content';
import { shot } from '@/data/site';

export default function Features() {
  return (
    <Shell as="section" id="players" className="pt-20 md:pt-28">
      <SectionMarker
        number="01"
        eyebrow="Players"
        title="Two tabs, one window"
        note="An image player and a video player, each aware of the folder the file came from."
      />

      <div className="flex flex-col gap-20 md:gap-28">
        {featureGroups.map((group, index) => {
          const flipped = index % 2 === 1;
          return (
            <div
              key={group.id}
              id={group.id}
              className="grid grid-cols-1 md:grid-cols-2 gap-10 md:gap-14 items-center"
            >
              <Reveal className={cn(flipped && 'md:order-2')}>
                <div className="flex items-baseline gap-3 mb-4">
                  <span className="fx-meta tabular text-ink-dim">{group.num}</span>
                  <group.icon size={16} strokeWidth={1.5} className="text-accent self-center" />
                  <h3 className="fx-display-i text-display-sm text-ink">{group.name}</h3>
                </div>

                <p className="fx-body text-ink-soft max-w-reading mb-8">{group.lede}</p>

                <ul className="border-t border-line">
                  {group.features.map((feature) => (
                    <li key={feature.title} className="feature-row border-b border-line-soft py-3.5">
                      <h4 className="fx-title-i text-[1.15rem] text-ink mb-0.5">{feature.title}</h4>
                      <p className="fx-body text-[0.9rem] text-ink-muted pretty">{feature.body}</p>
                    </li>
                  ))}
                </ul>
              </Reveal>

              <Reveal delay={120} className={cn(flipped && 'md:order-1')}>
                <SwapFrame
                  base={group.shot}
                  alt={group.shotAlt}
                  hover={group.hoverShot}
                  hoverLabel={group.hoverLabel}
                />
              </Reveal>
            </div>
          );
        })}
      </div>

      <Reveal className="mt-14 md:mt-20">
        <div className="frame">
          <img
            src={shot('video-controls')}
            alt="The Chitr control bar: play, timer, previous, open, next, and volume"
            className="block w-full h-auto"
          />
        </div>
        <p className="fx-meta text-ink-dim mt-2.5">
          The control bar — transport, timer, folder steps, volume
        </p>
      </Reveal>
    </Shell>
  );
}

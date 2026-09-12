import { ArrowDown, ArrowUpRight, Apple, Monitor, Terminal, Info } from 'lucide-react';
import Shell from './Shell';
import Reveal from './Reveal';
import SectionMarker from './SectionMarker';
import CopyBlock from './CopyBlock';
import { cn } from '@/lib/utils';
import { RELEASES_URL, REPO_URL } from '@/data/site';
import type { PlatformId, ReleaseState } from '@/lib/useRelease';

const ICONS: Record<PlatformId, typeof Apple> = {
  macos: Apple,
  windows: Monitor,
  linux: Terminal,
};

const ORDER: PlatformId[] = ['macos', 'windows', 'linux'];

interface DownloadProps {
  release: ReleaseState;
  detected: PlatformId | null;
}

export default function Download({ release, detected }: DownloadProps) {
  const published = release.publishedAt
    ? new Date(release.publishedAt).toLocaleDateString('en-GB', {
        day: 'numeric',
        month: 'long',
        year: 'numeric',
      })
    : null;

  return (
    <Shell as="section" id="download" className="pt-24 md:pt-32">
      <SectionMarker
        number="04"
        eyebrow="Download"
        title={`Chitr ${release.version}`}
        note={
          published
            ? `Published ${published}` : ``
        }
      />

      <div className="grid grid-cols-1 md:grid-cols-3 gap-5 md:gap-6">
        {ORDER.map((id, i) => {
          const download = release.downloads[id];
          const Icon = ICONS[id];
          const isDetected = detected === id;

          return (
            <Reveal key={id} delay={i * 100}>
              <a
                href={download.url}
                className={cn(
                  'group h-full flex flex-col border bg-paper-soft/60 p-7 transition-all duration-500',
                  'hover:-translate-y-1 hover:shadow-[0_16px_36px_rgba(20,20,26,0.12)] hover:bg-paper-soft',
                  isDetected ? 'border-accent' : 'border-line hover:border-ink-dim'
                )}
              >
                <div className="flex items-start justify-between gap-3 mb-6">
                  <Icon
                    size={22}
                    strokeWidth={1.4}
                    className="text-ink transition-transform duration-500 group-hover:-translate-y-1"
                  />
                  {isDetected && <span className="fx-eyebrow text-accent">Your system</span>}
                </div>

                <h3 className="fx-display-i text-[1.75rem] text-ink mb-1.5">{download.label}</h3>
                <p className="fx-meta text-ink-dim normal-case mb-6">{download.note}</p>

                <span className="mt-auto flex items-baseline justify-between gap-3 border-t border-line-soft pt-4">
                  <span className="fx-eyebrow text-ink group-hover:text-accent transition-colors inline-flex items-center gap-2">
                    Download
                    <ArrowDown size={13} strokeWidth={2} className="btn-arrow-down" />
                  </span>
                  <span className="fx-meta text-ink-dim tabular">{download.size ?? '—'}</span>
                </span>
              </a>
            </Reveal>
          );
        })}
      </div>

      <Reveal delay={120} className="mt-6 flex flex-wrap items-baseline gap-x-6 gap-y-2">
        <a
          href={RELEASES_URL}
          target="_blank"
          rel="noreferrer"
          className="fx-navlink fx-eyebrow text-ink inline-flex items-baseline gap-1"
        >
          All releases <ArrowUpRight size={12} strokeWidth={1.6} />
        </a>
        <a
          href={`${REPO_URL}/blob/main/LICENSE`}
          target="_blank"
          rel="noreferrer"
          className="fx-navlink fx-eyebrow text-ink-muted inline-flex items-baseline gap-1"
        >
          GPL-3.0 <ArrowUpRight size={12} strokeWidth={1.6} />
        </a>
      </Reveal>

      <div className="mt-14 grid grid-cols-1 lg:grid-cols-2 gap-6">
        <Reveal delay={60}>
          <div className="h-full border border-line bg-paper-soft/50 p-7">
            <div className="flex items-baseline gap-3 mb-3">
              <Info size={15} strokeWidth={1.5} className="text-accent translate-y-0.5 shrink-0" />
              <h3 className="fx-title-i text-[1.3rem] text-ink">First launch on macOS</h3>
            </div>
            <p className="fx-body text-[0.9rem] text-ink-muted pretty mb-5">
              The macOS builds are unsigned, so Gatekeeper refuses the first launch. Strip the
              quarantine attribute once, or right-click the app in Finder and choose{' '}
              <span className="fx-mark">Open</span>.
            </p>
            <CopyBlock command="xattr -dr com.apple.quarantine /path/to/Chitr.app" />
          </div>
        </Reveal>

        <Reveal delay={120}>
          <div className="h-full border border-line bg-paper-soft/50 p-7">
            <h3 className="fx-title-i text-[1.3rem] text-ink mb-3">Or build it yourself</h3>
            <p className="fx-body text-[0.9rem] text-ink-muted pretty mb-5">
              Conan 2 handles dependencies on macOS and Windows; on Linux, wxWidgets 3.2 comes from
              the system package manager.
            </p>
            <div className="flex flex-col gap-2.5">
              <CopyBlock command="git clone https://github.com/jamal474/chitr.git" />
              <CopyBlock command="conan install . --build=missing" />
              <CopyBlock command="conan build ." />
            </div>
          </div>
        </Reveal>
      </div>
    </Shell>
  );
}

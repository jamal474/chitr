import { ArrowUp, ArrowUpRight } from 'lucide-react';
import Shell from './Shell';
import Reveal from './Reveal';
import { asset } from '@/lib/utils';
import { roadmap } from '@/data/content';
import { AUTHOR, ISSUES_URL, RELEASES_URL, REPO_URL } from '@/data/site';

export default function SiteFooter() {
  return (
    <Shell as="footer" className="pb-16 pt-24 md:pt-32">
      <Reveal className="grid grid-cols-1 md:grid-cols-[1fr_1.4fr] gap-8 md:gap-16 items-start mb-20">
        <div>
          <p className="fx-eyebrow text-accent mb-3">Next up</p>
          <h2 className="fx-display-i text-display-sm text-ink">Still being written</h2>
        </div>
        <div>
          <ol className="border-t border-line">
            {roadmap.map((item, i) => (
              <li key={item} className="feature-row flex items-baseline gap-5 py-3 border-b border-line-soft">
                <span className="fx-meta tabular text-ink-dim shrink-0">
                  {String(i + 1).padStart(2, '0')}
                </span>
                <span className="fx-body text-[0.95rem] text-ink-soft">{item}</span>
              </li>
            ))}
          </ol>
          <a
            href={ISSUES_URL}
            target="_blank"
            rel="noreferrer"
            className="fx-navlink fx-eyebrow text-ink inline-flex items-baseline gap-1 mt-5"
          >
            Report a bug or request a feature <ArrowUpRight size={12} strokeWidth={1.6} />
          </a>
        </div>
      </Reveal>

      <hr className="fx-rule mb-10" />

      <div className="flex flex-col md:flex-row md:items-end md:justify-between gap-8">
        <div>
          <div className="flex items-center gap-3 mb-3">
            <img src={asset('icon.png')} alt="" className="size-8 border border-line" />
            <span className="fx-display-i text-[1.75rem] text-ink leading-none">Chitr</span>
          </div>
          <p className="fx-body text-[0.9rem] text-ink-muted max-w-[38ch]">
            A cross-platform media player written in C++. Free software under the GPL-3.0.
          </p>
        </div>

        <nav className="flex flex-wrap items-baseline gap-x-6 gap-y-2">
          <a href={REPO_URL} target="_blank" rel="noreferrer" className="fx-navlink text-ink hover:text-accent">
            github
          </a>
          <a href={RELEASES_URL} target="_blank" rel="noreferrer" className="fx-navlink text-ink hover:text-accent">
            releases
          </a>
          <a href={AUTHOR.site} target="_blank" rel="noreferrer" className="fx-navlink text-ink hover:text-accent">
            shabbirjamal.com
          </a>
          <a href="#top" className="fx-navlink text-ink hover:text-accent inline-flex items-baseline gap-1.5">
            top <ArrowUp size={12} strokeWidth={1.6} />
          </a>
        </nav>
      </div>

      <hr className="fx-rule-soft my-8" />

      <p className="fx-meta text-ink-dim normal-case">
        Built by{' '}
        <a href={AUTHOR.site} target="_blank" rel="noreferrer" className="fx-navlink text-ink-muted">
          {AUTHOR.name}
        </a>
      </p>
    </Shell>
  );
}

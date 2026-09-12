import { ArrowDown, Github } from 'lucide-react';
import { cn, asset } from '@/lib/utils';
import { REPO_URL } from '@/data/site';
import { useActiveSection } from '@/lib/useActiveSection';
import { useScrolled } from '@/lib/useScrolled';

const NAV = [
  { id: 'players', num: '01', label: 'Players' },
  { id: 'demo', num: '02', label: 'Demo' },
  { id: 'reference', num: '03', label: 'Reference' },
  { id: 'download', num: '04', label: 'Download' },
];

const NAV_IDS = NAV.map((n) => n.id);

/** Sticky masthead that compacts and gains a backdrop once the page moves. */
export default function SiteHeader() {
  const scrolled = useScrolled(40);
  const active = useActiveSection(NAV_IDS);

  return (
    <header
      className={cn(
        'sticky top-0 z-40 transition-[background-color,backdrop-filter,border-color,padding] duration-500 ease-out border-b',
        scrolled
          ? 'bg-paper/85 backdrop-blur-md border-line py-3'
          : 'bg-transparent border-transparent py-5 md:py-7'
      )}
    >
      <div className="mx-auto w-full max-w-shell px-5 md:px-shell-x flex items-center justify-between gap-6">
        <a href="#top" className="flex items-center gap-3 group shrink-0">
          <img
            src={asset('icon.png')}
            alt=""
            className={cn(
              'border border-line transition-all duration-500 ease-out',
              scrolled ? 'size-7' : 'size-8 md:size-9'
            )}
          />
          <span
            className={cn(
              'fx-display-i text-ink leading-none transition-all duration-500 ease-out',
              scrolled ? 'text-[1.25rem]' : 'text-[1.5rem] md:text-[1.75rem]'
            )}
          >
            Chitr
          </span>
        </a>

        <nav aria-label="Sections" className="hidden md:block">
          <ol className="flex items-baseline gap-x-6">
            {NAV.map((item) => (
              <li key={item.id} className="flex items-baseline gap-1.5">
                <span
                  className={cn(
                    'fx-meta tabular transition-colors',
                    active === item.id ? 'text-accent' : 'text-ink-dim'
                  )}
                >
                  {item.num}
                </span>
                <a
                  href={`#${item.id}`}
                  aria-current={active === item.id}
                  className={cn(
                    'fx-navlink fx-title-i text-[0.95rem]',
                    active === item.id ? 'text-accent' : 'text-ink-soft'
                  )}
                >
                  {item.label}
                </a>
              </li>
            ))}
          </ol>
        </nav>

        <div className="flex items-center gap-4 shrink-0">
          <a
            href={REPO_URL}
            target="_blank"
            rel="noreferrer"
            aria-label="Chitr on GitHub"
            className="text-ink-muted hover:text-accent transition-colors hover:-translate-y-0.5 duration-300"
          >
            <Github size={18} strokeWidth={1.5} />
          </a>
          <a href="#download" className="btn !py-2 !px-4 fx-eyebrow">
            Download
            <ArrowDown size={13} strokeWidth={2} className="btn-arrow-down" />
          </a>
        </div>
      </div>
    </header>
  );
}

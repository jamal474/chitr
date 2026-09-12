import { cn } from '@/lib/utils';
import Reveal from './Reveal';

interface SectionMarkerProps {
  number: string;
  eyebrow: string;
  title: string;
  note?: string;
  className?: string;
}

/** The opening block of a section: § number, eyebrow, title, rule. */
export default function SectionMarker({ number, eyebrow, title, note, className }: SectionMarkerProps) {
  return (
    <Reveal className={cn('mb-12 md:mb-16', className)}>
      <div className="flex items-baseline gap-3 mb-3">
        <span className="fx-meta tabular text-ink-dim">§ {number}</span>
        <span className="fx-eyebrow text-accent">{eyebrow}</span>
      </div>
      <h2 className="fx-display-i text-display-md text-ink">{title}</h2>
      {note && <p className="fx-body text-ink-muted mt-4 max-w-reading pretty">{note}</p>}
      <hr className="fx-rule mt-8 md:mt-10" />
    </Reveal>
  );
}

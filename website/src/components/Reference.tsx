import { useState } from 'react';
import { AnimatePresence, motion } from 'framer-motion';
import Shell from './Shell';
import Reveal from './Reveal';
import SectionMarker from './SectionMarker';
import { cn } from '@/lib/utils';
import { formatNote, formatTables, shortcutGroups } from '@/data/content';

type TabId = 'formats' | 'shortcuts';

const TABS: { id: TabId; label: string }[] = [
  { id: 'formats', label: 'Supported formats' },
  { id: 'shortcuts', label: 'Keyboard shortcuts' },
];

export default function Reference() {
  const [tab, setTab] = useState<TabId>('formats');

  return (
    <Shell as="section" id="reference" className="pt-24 md:pt-32">
      <SectionMarker
        number="03"
        eyebrow="Reference"
        title="Formats and keys"
        note="Everything Chitr opens, and every key that drives it."
      />

      <Reveal>
        <div role="tablist" aria-label="Reference tables" className="flex flex-wrap gap-x-8 gap-y-1 border-b border-line mb-10">
          {TABS.map((item) => (
            <button
              key={item.id}
              role="tab"
              aria-selected={tab === item.id}
              onClick={() => setTab(item.id)}
              className={cn(
                'relative pb-3 fx-eyebrow transition-colors duration-300',
                tab === item.id ? 'text-accent' : 'text-ink-dim hover:text-ink'
              )}
            >
              {item.label}
              {tab === item.id && (
                <motion.span
                  layoutId="reference-tab"
                  className="absolute left-0 right-0 -bottom-px h-[2px] bg-accent"
                  transition={{ type: 'spring', stiffness: 380, damping: 32 }}
                />
              )}
            </button>
          ))}
        </div>
      </Reveal>

      <AnimatePresence mode="wait">
        <motion.div
          key={tab}
          initial={{ opacity: 0, y: 8 }}
          animate={{ opacity: 1, y: 0 }}
          exit={{ opacity: 0, y: -8 }}
          transition={{ duration: 0.28, ease: [0.2, 0.7, 0.1, 1] }}
        >
          {tab === 'formats' ? <Formats /> : <Shortcuts />}
        </motion.div>
      </AnimatePresence>
    </Shell>
  );
}

function Formats() {
  return (
    <div>
      <div className="grid grid-cols-1 lg:grid-cols-3 gap-10 lg:gap-12">
        {formatTables.map((table) => (
          <div key={table.id}>
            <h3 className="fx-title-i text-[1.4rem] text-ink mb-1">{table.label}</h3>
            <hr className="fx-rule mb-5" />
            <dl className="flex flex-col gap-5">
              {table.rows.map((row) => (
                <div key={row.platform}>
                  <dt className="fx-meta text-ink-dim mb-2.5">{row.platform}</dt>
                  <dd className="flex flex-wrap gap-1.5">
                    {row.extensions.map((ext) => (
                      <span
                        key={ext}
                        className="fx-meta text-ink px-2 py-1 border border-line bg-paper-soft transition-colors duration-300 hover:border-accent hover:text-accent"
                      >
                        {ext}
                      </span>
                    ))}
                  </dd>
                </div>
              ))}
            </dl>
          </div>
        ))}
      </div>
      <p className="fx-body text-[0.9rem] text-ink-muted mt-12 max-w-reading pretty border-l-2 border-line pl-5">
        {formatNote}
      </p>
    </div>
  );
}

function Shortcuts() {
  return (
    <div className="grid grid-cols-1 lg:grid-cols-2 gap-10 lg:gap-16">
      {shortcutGroups.map((group) => (
        <div key={group.id}>
          <h3 className="fx-title-i text-[1.4rem] text-ink mb-1">{group.label}</h3>
          <hr className="fx-rule mb-2" />
          <ul>
            {group.shortcuts.map((shortcut) => (
              <li
                key={`${group.id}-${shortcut.action}-${shortcut.keys.join('')}`}
                className="grid grid-cols-[auto_1fr] gap-x-5 gap-y-1 items-baseline py-3 border-b border-line-soft transition-colors duration-300 hover:bg-paper-soft/70"
              >
                <span className="flex flex-wrap items-center gap-1 sm:min-w-[7.5rem]">
                  {shortcut.keys.map((key, i) =>
                    key === '+' || key === '–' ? (
                      <span key={i} className="fx-meta text-ink-dim px-0.5">
                        {key}
                      </span>
                    ) : (
                      <kbd key={i} className="kbd">
                        {key}
                      </kbd>
                    )
                  )}
                </span>
                <span className="fx-body text-[0.95rem] text-ink">{shortcut.action}</span>
                <span />
                <span className="fx-meta text-ink-dim normal-case">{shortcut.description}</span>
              </li>
            ))}
          </ul>
        </div>
      ))}
    </div>
  );
}

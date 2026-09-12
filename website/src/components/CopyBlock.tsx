import { useState } from 'react';
import { Check, Copy } from 'lucide-react';
import { cn } from '@/lib/utils';

interface CopyBlockProps {
  command: string;
  label?: string;
  className?: string;
}

/** A shell line with a copy button that confirms for a moment. */
export default function CopyBlock({ command, label, className }: CopyBlockProps) {
  const [copied, setCopied] = useState(false);

  async function copy() {
    try {
      await navigator.clipboard.writeText(command);
      setCopied(true);
      window.setTimeout(() => setCopied(false), 1800);
    } catch {
      setCopied(false);
    }
  }

  return (
    <div className={cn(className)}>
      {label && <p className="fx-meta text-ink-dim mb-2">{label}</p>}
      <div className="group flex items-center gap-3 border border-line bg-paper-soft px-4 py-3 transition-colors duration-300 hover:border-ink-dim">
        <span className="text-accent font-mono text-[0.8rem] select-none">$</span>
        <code className="font-mono text-[0.8rem] text-ink-soft overflow-x-auto no-scrollbar whitespace-nowrap flex-1 min-w-0">
          {command}
        </code>
        <button
          type="button"
          onClick={copy}
          aria-label={copied ? 'Copied' : 'Copy command'}
          className="shrink-0 text-ink-dim hover:text-accent transition-colors"
        >
          <span className="relative block size-4">
            <Copy
              size={16}
              strokeWidth={1.5}
              className={cn(
                'absolute inset-0 transition-all duration-300',
                copied ? 'opacity-0 scale-50' : 'opacity-100 scale-100'
              )}
            />
            <Check
              size={16}
              strokeWidth={2}
              className={cn(
                'absolute inset-0 text-accent transition-all duration-300',
                copied ? 'opacity-100 scale-100' : 'opacity-0 scale-50'
              )}
            />
          </span>
        </button>
      </div>
    </div>
  );
}

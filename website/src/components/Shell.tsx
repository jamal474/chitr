import { cn } from '@/lib/utils';

interface ShellProps {
  children: React.ReactNode;
  className?: string;
  as?: keyof JSX.IntrinsicElements;
  id?: string;
}

/** The shared centered container — one width, one horizontal padding. */
export default function Shell({ children, className, as: Tag = 'div', id }: ShellProps) {
  return (
    <Tag id={id} className={cn('mx-auto w-full max-w-shell px-5 md:px-shell-x', className)}>
      {children}
    </Tag>
  );
}

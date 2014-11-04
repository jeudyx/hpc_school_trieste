PROGRAM bench_daxpy

  IMPLICIT NONE

  INTEGER, PARAMETER :: dimx = 25000000
  REAL*8, ALLOCATABLE :: x(:), y(:)
  INTEGER :: i,j,k
  INTEGER :: idatetime(8)
  CHARACTER(LEN=8)  :: adate
  CHARACTER(LEN=10) :: atime
  CHARACTER(LEN=256) :: filename
  REAL*8 :: t1, t2
  REAL*8 :: cclock
  EXTERNAL :: cclock

  ALLOCATE( x( dimx ), y( dimx ) )

  y = 1.0d0
  x = 1.0d0 / DBLE( dimx )

  t1 = cclock()

  call daxpy( dimx, 1.0d0, x, 1, y, 1 )

  t2 = cclock()

  write(*,65) 'bench_daxpy (size)     = ', dimx
  write(*,70) 'bench_daxpy (seconds)  = ', t2-t1
  write(*,75) 'bench_daxpy (Gflops)   = ', 2.0d0 * dimx / 1.0d9 / (t2-t1)
  write(*,75) 'bench_daxpy (Gbsec)    = ', 8.0d0 * 2.0d0 * dimx / 1.0d9 / (t2-t1)

65 FORMAT(A,I15)
70 FORMAT(A,F16.6)
75 FORMAT(A,F10.4)

  close(unit=10)

  DEALLOCATE( x, y )

END PROGRAM bench_daxpy

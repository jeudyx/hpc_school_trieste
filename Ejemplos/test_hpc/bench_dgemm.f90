PROGRAM bench_dgemm

  IMPLICIT NONE

  INTEGER, PARAMETER :: dim = 4096

  REAL*8, ALLOCATABLE :: x(:,:), y(:,:), z(:,:)
  INTEGER :: i,j,k
  REAL*8 :: t1, t2
  INTEGER :: idatetime(8)

  INTEGER :: NTHREADS=0, TID, OMP_GET_NUM_THREADS, OMP_GET_THREAD_NUM
  REAL*8 :: cclock
  EXTERNAL :: cclock


  ALLOCATE( x( dim, dim ), y( dim, dim ), z( dim, dim ) )

  y = 1.0d0 / ( DBLE( dim ) )
  z = 1.0d0 / ( DBLE( dim ) )
  x = 0.0d0

  t1 = cclock()

  call dgemm('N', 'N', dim, dim, dim, 1.0d0, y, dim, z, dim, &
       &           0.0d0, x, dim)

  t2 = cclock()

  !$OMP PARALLEL SHARED(NTHREADS), PRIVATE(TID)
  TID = OMP_GET_THREAD_NUM()
  IF (TID .EQ. 0) THEN
     NTHREADS = OMP_GET_NUM_THREADS()
  END IF
  !$OMP END PARALLEL

  write(*,65) 'bench_dgemm (threads)  = ', NTHREADS
  write(*,65) 'bench_dgemm (size)     = ', dim
  write(*,70) 'bench_dgemm (seconds)  = ', t2-t1
  write(*,75) 'bench_dgemm (Gflops)   = ', 2.0d0 * DBLE(dim)**3 / 1.0d9 / (t2-t1)

65 FORMAT(A,I4)
70 FORMAT(A,F16.6)
75 FORMAT(A,F10.4)

  DEALLOCATE( x, y, z )

END PROGRAM bench_dgemm

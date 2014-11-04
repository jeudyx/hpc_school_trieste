PROGRAM bench_mpi_daxpy

  IMPLICIT NONE

  INTEGER, PARAMETER :: dimx = 25000000

  REAL*8, ALLOCATABLE :: x(:), y(:)
  INTEGER :: i,j,k,n
  INTEGER :: nproc, mpime, root, group, ierr
  CHARACTER(LEN=8)  :: adate
  CHARACTER(LEN=10) :: atime
  CHARACTER(LEN=256) :: filename
  REAL*8 :: t1, t2
  REAL*8 :: flops, tot_flops, bandwidth, tot_bandwidth 
  REAL*8 :: cclock
  EXTERNAL :: cclock

  CALL startup(nproc, mpime, root, group)

  ALLOCATE( x( dimx ), y( dimx ) )

  y = 1.0d0
  x = 1.0d0 / DBLE( dimx )
  n = 10

  CALL SYNCRONIZE()
  t1 = cclock()
  DO i = 1, n
     call daxpy( dimx, 1.0d0, x, 1, y, 1 )
  END DO
  CALL SYNCRONIZE()
  t2 = cclock()

  flops = n * 2.0d0 * dimx / 1.0d9 / (t2 - t1)
  bandwidth = n * 8.0d0 * 2.0d0 * dimx / 1.0d9 / (t2 - t1)

  CALL COMPUTE_AGGREGATE( flops, tot_flops )
  CALL COMPUTE_AGGREGATE( bandwidth, tot_bandwidth )

  IF( mpime == 0 ) THEN
     write(*,65) 'bench_mpi_daxpy (nproc)    = ', nproc
     write(*,65) 'bench_mpi_daxpy (size)     = ', dimx
     write(*,70) 'bench_mpi_daxpy (seconds)  = ', t2-t1
     write(*,75) 'bench_mpi_daxpy (Gflops)   = ', flops
     write(*,75) 'bench_mpi_daxpy (Gbsec)    = ', bandwidth
     write(*,75) 'bench_mpi_daxpy (Aggregate Gflops)   = ', tot_flops
     write(*,75) 'bench_mpi_daxpy (Aggregate Gbsec)    = ', tot_bandwidth
  END IF

65 FORMAT(A,I15)
70 FORMAT(A,F16.6)
75 FORMAT(A,F10.4)

  CALL SYNCRONIZE()
  CALL HANGUP()

  DEALLOCATE( x, y )

END PROGRAM bench_mpi_daxpy

! ------------------------------------------------------- !

SUBROUTINE SYNCRONIZE()  
  IMPLICIT NONE
#if defined __MPI
  include 'mpif.h'
  INTEGER :: IERR
  CALL MPI_BARRIER(MPI_COMM_WORLD, IERR)
#endif
  RETURN 
END SUBROUTINE SYNCRONIZE

SUBROUTINE COMPUTE_AGGREGATE( unit, tot_units )
  IMPLICIT NONE
#if defined __MPI
  include 'mpif.h'
  INTEGER :: IERR
  REAL*8 :: unit, tot_units
  call MPI_REDUCE( unit, tot_units, 1, MPI_DOUBLE_PRECISION, MPI_SUM, 0, MPI_COMM_WORLD, IERR )
#else
  tot_units = unit
#endif
  RETURN 
END SUBROUTINE

SUBROUTINE hangup
  IMPLICIT NONE
#if defined __MPI
  INCLUDE 'mpif.h'
  INTEGER ierr
  CALL MPI_Finalize(ierr)
#endif
  RETURN
END SUBROUTINE hangup

SUBROUTINE startup(nproc, mpime, root, group)
  IMPLICIT NONE

#if defined __MPI
  INCLUDE 'mpif.h'
#endif

  INTEGER nproc
  INTEGER mpime, i, err, root, group

#if defined __MPI
  CALL MPI_Init(err)
  CALL MPI_Comm_size(MPI_COMM_WORLD, nproc, err)
  CALL MPI_Comm_rank(MPI_COMM_WORLD, mpime, err)
  root = 0
  group = MPI_COMM_WORLD
#else
  nproc = 1
  root = 0
  mpime = 0
  group = 0
#endif

  RETURN
END SUBROUTINE startup


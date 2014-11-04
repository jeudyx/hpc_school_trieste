!
!     It evolves the equation:
!                             u,t + u,x + u,y = 0
!     Using a Lax scheme.
!     The initial data is a cruddy gaussian distributed by columns
!     and data allocated dynamically
!     Using periodic boundaries     

module transport
  implicit none
  save

  integer, parameter :: NX=400
  integer, parameter :: NY=400
  real, parameter    :: LX=2.0
  real, parameter    :: LY=2.0

  integer, parameter :: STEPS=1200
  real, parameter    :: DT=0.1

  real temp(0:NX+1 , 0:NY+1)
  real temp_new(0:NX+1 , 0:NY+1)


CONTAINS
  !conversions from discrete to real coordinates

  real function ix2x(ix)
    integer ix
    ix2x = ((ix-1)-(NX-1)/ 2.0)*LX/(NX-1)
  end function ix2x

  real function iy2y(iy)
    integer iy
    iy2y = ((iy-1)-(NY-1)/ 2.0)*LY/(NY-1)
  end function iy2y


 ! initialize the system with a gaussian temperature distribution

  subroutine init_transport
    integer ix,iy
    real x,y
    real,parameter :: sigma = 0.1
    real,parameter :: tmax = 100

    do iy=1,NY
       do ix=1,NX
          x=ix2x(ix)
          y=iy2y(iy)
          temp(ix,iy) = tmax*exp(-(x**2+y**2)/(2.0*sigma**2))
       enddo
    enddo

  end subroutine init_transport

  ! save the temperature distribution
  ! the ascii format is suitable for splot gnuplot function
  subroutine save_gnuplot(filename)

    character(len=*) filename
    integer ix,iy

    open(unit=20,file=filename,form='formatted')
    do iy=1,NY
       do ix=1,NX
          write(20,*) ix2x(ix),iy2y(iy),temp(ix,iy)
       enddo
       write(20,*)
    enddo

    close(20)
  end subroutine save_gnuplot


  SUBROUTINE update_boundaries_PBC
    temp(   0,1:NY) = temp(  NX,1:NY)
    temp(NX+1,1:NY) = temp(   1,1:NY)
    temp(1:NX,   0) = temp(1:NX,  NY)
    temp(1:NX,NY+1) = temp(1:NX,   1)

    temp(   0,   0) = temp(  NX,  NY)
    temp(NX+1,   0) = temp(   1,  NY)
    temp(   0,NY+1) = temp(  NX,   1)
    temp(NX+1,NY+1) = temp(   1,   1)
  END SUBROUTINE update_boundaries_PBC

  subroutine evolve(dtfact)
    real :: dtfact
    real :: dx,dt
    integer :: ix,iy
    real :: temp0


    dx = 2*LX/NX
    dt = dtfact*dx/sqrt(3.0)
    do iy=1,NY
       do ix=1,NX
          temp0 = temp(ix,iy)
          temp_new(ix,iy) = temp0 - 0.5 * dt * (temp(ix+1,iy)-temp(ix-1,iy)+temp(ix,iy+1)-temp(ix,iy-1)) / dx
       enddo
    enddo

    temp = temp_new

  end subroutine evolve
end module transport

program run_transport
  use transport
  implicit none

  integer :: i
  real :: tinit, tstart, tstop, tio

  CALL CPU_TIME(tinit)

  call init_transport
  call update_boundaries_PBC

#if defined __DEBUG
  write(*,*) 'sum temp before',sum(temp(1:NX,1:NY))
#endif

  CALL CPU_TIME(tstop)

  PRINT *,'initialization done'
  PRINT *,'cpu time in seconds ', tstop-tinit

  CALL CPU_TIME(tstart)
  call save_gnuplot('transport.dat')
  CALL CPU_TIME(tstop)

  tio = tstop - tstart

  CALL CPU_TIME(tstart)

  do i=1,STEPS
     call evolve(DT)
     call update_boundaries_PBC
  enddo
  
  CALL CPU_TIME(tstop)
  
  PRINT *,'evolution done'
  PRINT *,'cpu time in seconds ', tstop-tstart
  
  CALL CPU_TIME(tstart)
  CALL save_gnuplot('transport-end-serial.dat')
  CALL CPU_TIME(tstop)

  tio = tio + tstop - tstart
  PRINT *,'save_data done'
  PRINT *,'IO time in seconds ', tio
     
  PRINT *,'total cpu time in seconds ', tstop - tinit
  
#if defined __DEBGU
  write(*,*) 'sum temp after',sum(temp(1:NX,1:NY))
#endif

end program run_transport

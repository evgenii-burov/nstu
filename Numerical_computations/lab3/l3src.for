      program main
      common/size/n
        dimension mem(1000000)
        call readDimension()
        call readVector(mem(1))
        call printVector(mem(1))
        
        pause
      end
      
      subroutine readDimension()
        common/size/n
        n=-1
        open(1,file='dimension.txt', status='old', err=1)
        read(1,*)n
        close(1)
        if(n.lt.1)then
            print*,'Incorrect parameter: n must be positive'
            pause
            stop
        endif
        return
        
    1   print*,'Couldnt open dimension file'
        pause 
        stop
      end
      
      subroutine readMatrix(di, al, au, ja, ia)
      common/size/n
        dimension di(*), al(*), au(*), ja(*), ia(*)
        
        open(1,file='di.txt', status='old', err=1)
        read(1,*)(di(i), i=1,n)
        close(1)
        
        open(1,file='al.txt', status='old', err=1)
        read(1,*)(v(i), i=1,n)
        close(1)
        
        open(1,file='au.txt', status='old', err=1)
        read(1,*)(v(i), i=1,n)
        close(1)
        
        open(1,file='ja.txt', status='old', err=1)
        read(1,*)(v(i), i=1,n)
        close(1)
        
        open(1,file='ia.txt', status='old', err=1)
        read(1,*)(v(i), i=1,n)
        close(1)
        
        return
    1   print*,'Couldnt open vector file'
        pause 
        stop      
      end
      
      subroutine readVector(v)
        common/size/n
        dimension v(*)
        open(1,file='vector.txt', status='old', err=1)
        read(1,*)(v(i), i=1,n)
        close(1)
        return
    1   print*,'Couldnt open vector file'
        pause 
        stop      
      end
      
      subroutine printVector(v)
        common/size/n
        dimension v(*)
        do i=1,n
          print*,v(i)
        enddo
      end

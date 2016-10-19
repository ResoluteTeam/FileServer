class AttachmentUploader < CarrierWave::Uploader::Base
   storage :file

   def store_dir
      "uploads/#{model.class.to_s.underscore}/#{mounted_as}/#{model.id}"
   end

   def extension_white_list
      %w(cpp pdf doc htm html docx)
   end
end